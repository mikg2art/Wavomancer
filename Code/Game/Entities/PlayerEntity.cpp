#include "PlayerEntity.h"

#include "Engine/EntitySystem/EntityFactory.h"
#include "Engine/EntitySystem/Components/SpriteComponent.h"
#include "Engine/EntitySystem/Components/RigidbodyComponent.h"
#include "Engine/EntitySystem/Components/CircleCollisionComponent.h"
#include "Engine/EntitySystem/Components/ViewComponent.h"
#include "Engine/Engine.h"

#include "Game/Entities/ProjectileEntity.h"
#include "Game/Entities/HealthbarEntity.h"
#include "Game/Entities/GameManager.h"
#include "Game/GameStates.h"

#include "Engine/SFMLMath/SFMLMath.hpp"

using namespace sf;

void PlayerEntity::Init()
{
	//Player
	m_pPlayerSpriteComponent = AddComponent<SpriteComponent>();
	m_pPlayerSpriteComponent->SetSpriteByPath("../Assets/Kenney/Dungeon/Tiles/tile_0111.png", true);
	m_pPlayerSpriteComponent->GetSprite().setScale(3.f, 3.f);

	//Weapon
	m_pWeaponSpriteComponent = AddComponent<SpriteComponent>();
	m_pWeaponSpriteComponent->SetSpriteByPath("../Assets/Kenney/Dungeon/Tiles/d_tile_0130.png", false);
	m_pWeaponSpriteComponent->GetSprite().setScale(3.f, 3.f);
	m_pWeaponSpriteComponent->GetSprite().setPosition(m_pPlayerSpriteComponent->GetSprite().getPosition() - Vector2f(15.f, -10.f));
	m_pWeaponSpriteComponent->GetSprite().setOrigin(sf::Vector2f(7.f, 14.f));

	//CollisionComponent
	m_pCollisionComponent = AddComponent<CircleCollisionComponent>();
	m_pCollisionComponent->SetProperties(20.f, false, false, nullptr);

	//HealthbarEntity
	m_pHealthbarEntity = Engine::GetInstance()->GetEntitySystem().SpawnEntity<HealthbarEntity>();
	m_pHealthbarEntity->SetOwnerEntity(GetId());
	m_pHealthbarEntity->SetOffset(sf::Vector2f(0.f, -75.f));

	m_currentHealth = m_maxHealth;

	//Buffs
	std::shared_ptr<MovementSpeedBuff> pBaseMoveSpeedBuff = std::make_shared<MovementSpeedBuff>();
	pBaseMoveSpeedBuff->flatMoveSpeedIncrease = 1200.f;
	AddBuff(pBaseMoveSpeedBuff);

	std::shared_ptr<BulletCountBuff> pBaseBulletCountBuff = std::make_shared<BulletCountBuff>();
	pBaseBulletCountBuff->additionalBulletsPerFire = 1;
	AddBuff(pBaseBulletCountBuff);

	ReevaluateStats();
}


void PlayerEntity::Update(float deltaSeconds)
{
	if (m_handleInput)
	{
		UpdateMovement(GetMovementVector(), deltaSeconds);
	}
	UpdateWeaponRotation(deltaSeconds);

	if (m_fireTimer >= 0) {
		m_fireTimer -= deltaSeconds;
	}
	else m_canShoot = true;

	if (m_shooting) {
		if (m_canShoot) {
			OnInputPressed();
			m_fireTimer = m_fireRate;
			m_canShoot = false;
		}
	}

	UpdateEnemyIndicatorRotation(deltaSeconds);
	UpdateBuffs(deltaSeconds);
}


void PlayerEntity::Shutdown()
{
	if (m_inputCallbackId != 0)
	{
		Engine::GetInstance()->GetInputManager().Unregister(m_inputCallbackId);
		m_inputCallbackId = 0;
	}

	if (m_pHealthbarEntity)
	{
		m_pHealthbarEntity->Destroy();
		m_pHealthbarEntity = nullptr;
	}
}

void PlayerEntity::ProcessDamage(float damageAmount)
{
	for (const std::shared_ptr<Buff>& pBuff : m_buffs)
	{
		pBuff->EvaluateStat(EBuffStat::Armor, damageAmount);
	}

	if (damageAmount <= 0.f)
		return;

	m_currentHealth = std::max(0.f, m_currentHealth - damageAmount);

	if (m_currentHealth <= 0.f)
	{
		m_handleInput = false;
		m_pWeaponSpriteComponent->SetVisibility(false);

		std::shared_ptr<GameOverGameState> pGameOverState = std::make_shared<GameOverGameState>();
		pGameOverState->loserEntityId = GetId();
		GameManager::Get()->SetGameState(pGameOverState);
	}
}

void PlayerEntity::SetInputKeys(sf::Mouse::Button fireButton, const TMovementKeys& movementKeys)
{
	if (m_inputCallbackId != 0)
	{
		Engine::GetInstance()->GetInputManager().Unregister(m_inputCallbackId);
		m_inputCallbackId = 0;
	}

	m_inputCallbackId = Engine::GetInstance()->GetInputManager().Register(fireButton, EInputEvent::Pressed, std::bind(&PlayerEntity::Shooting, this));
	m_inputCallbackId = Engine::GetInstance()->GetInputManager().Register(fireButton, EInputEvent::Released, std::bind(&PlayerEntity::NotShooting, this));
	m_movementKeys = movementKeys;
}

void PlayerEntity::SetColor(sf::Color color)
{
	m_factionColor = color;

	m_pPlayerSpriteComponent->GetSprite().setColor(m_factionColor);
	m_pWeaponSpriteComponent->GetSprite().setColor(m_factionColor);
}

void PlayerEntity::SetTargetId(int targetEntityId, sf::Color color)
{
	m_targetEntityId = targetEntityId;
	m_pEnemyIndicatorSpriteComponent->GetSprite().setColor(color);
}

void PlayerEntity::AddBuff(std::shared_ptr<Buff> pBuff)
{
	if (pBuff)
	{
		m_buffs.push_back(pBuff);
		ReevaluateStats();
	}
}

void PlayerEntity::UpdateMovement(sf::Vector2f movementVector, float deltaSeconds)
{
	m_mult = 1.f;

	if (std::abs(movementVector.x) + std::abs(movementVector.y) > 1) {
		m_mult /= std::sqrt(2.f);
	}

	const sf::Vector2f oldPosition = GetPosition();
	const sf::Vector2f newPosition = GetPosition() + (movementVector * deltaSeconds * m_movementSpeed * m_mult);
	SetPosition(newPosition);
}

void PlayerEntity::UpdateBuffs(float deltaSeconds)
{
	for (int i = static_cast<int>(m_buffs.size()) - 1; i >= 0; i--)
	{
		std::shared_ptr<Buff> pBuff = m_buffs[i];

		if (pBuff->lifetime < 0.f)
		{
			continue;
		}

		pBuff->lifetime -= deltaSeconds;
		if (pBuff->lifetime <= 0.f)
		{
			m_buffs.erase(m_buffs.begin() + i);
		}
	}

	ReevaluateStats();
}

void PlayerEntity::ReevaluateStats()
{
	m_movementSpeed = std::min(ReevaluateStat(EBuffStat::MoveSpeed), 500.f);
	m_bulletsPerShot = std::min(static_cast<int>(ReevaluateStat(EBuffStat::BulletCount)), 5);
}

float PlayerEntity::ReevaluateStat(EBuffStat stat)
{
	float value = 0.f;
	for (std::shared_ptr<Buff> pBuff : m_buffs)
	{
		pBuff->EvaluateStat(stat, value);
	}

	return value;
}

sf::Vector2f PlayerEntity::GetMovementVector() const
{

	InputManager& manager = Engine::GetInstance()->GetInputManager();

	float movementSpeed = 200.f;
	sf::Vector2f movementVector;
	if (m_canMove) {
		// vertical
		if (manager.IsKeyPressed(m_movementKeys[0]))
		{
			movementVector.y -= 1.f;
		}

		if (manager.IsKeyPressed(m_movementKeys[2]))
		{
			movementVector.y += 1.f;
		}

		// horizontal
		if (manager.IsKeyPressed(m_movementKeys[1]))
		{
			movementVector.x -= 1.f;
		}

		if (manager.IsKeyPressed(m_movementKeys[3]))
		{
			movementVector.x += 1.f;
		}
	}

	return movementVector;
}


void PlayerEntity::UpdateWeaponRotation(float deltaSeconds)
{
	sf::RenderWindow& renderWindow = Engine::GetInstance()->GetRenderWindow();
	sf::Vector2i mouseScreenPosition = sf::Mouse::getPosition(renderWindow);

	sf::Vector2f mouseWorldPosition = renderWindow.mapPixelToCoords(mouseScreenPosition);
	sf::Vector2f turrentPosition = GetPosition();

	const float degree = static_cast<float>(sf::radToDeg(std::atan2f(mouseWorldPosition.x - turrentPosition.x, mouseWorldPosition.y - turrentPosition.y))) * -1.f;
	m_pWeaponSpriteComponent->GetSprite().setRotation(degree+180.f);
}


void PlayerEntity::UpdateEnemyIndicatorRotation(float deltaSeconds)
{
	if (Entity* pTargetEntity = Engine::GetInstance()->GetEntitySystem().FindEntityById(m_targetEntityId))
	{
		m_pEnemyIndicatorSpriteComponent->GetSprite().setRotation(sf::GetRotationFromDirection(GetPosition() - pTargetEntity->GetPosition()));
	}
}

void PlayerEntity::Shooting() { m_shooting = true; }

void PlayerEntity::NotShooting() { m_shooting = false; }

void PlayerEntity::SetCanMove(bool canMove) { m_canMove = canMove; }

void PlayerEntity::SetFireRate(float fireRate)
{
	int help = fireRate * 100;
	float help2 = help / 100;
	std::cout << fireRate/10 << std::endl;
	m_fireRate = 0.2f - fireRate/10;
}

void PlayerEntity::OnInputPressed()
{
	if (!m_handleInput)
		return;

	constexpr float bulletOffset = 20.f;
	const float positionOffset = (m_bulletsPerShot - 1) * bulletOffset * 0.5f;

	const float turretRotationInDegrees = m_pWeaponSpriteComponent->GetSprite().getRotation()-180.f;
	sf::Vector2f fireVector = sf::GetDirectionFromRotation(turretRotationInDegrees);
	sf::Vector2f rightVector = sf::GetDirectionFromRotation(turretRotationInDegrees + 90);

	for (int i = 0; i < m_bulletsPerShot; i++)
	{
		ProjectileEntity* pBulletEntity = Engine::GetInstance()->GetEntitySystem().SpawnEntity<ProjectileEntity>();

		const sf::Vector2f newPosition = GetPosition() + fireVector * 61.f + (-rightVector * positionOffset) + (rightVector * bulletOffset * static_cast<float>(i));
		pBulletEntity->SetPosition(newPosition);
		pBulletEntity->SetRotation(sf::GetRotationFromDirection(newPosition - GetPosition()));
		
		pBulletEntity->Launch(750.f, 2.f, 20.f, 5, m_factionColor, GetId());
	}
}