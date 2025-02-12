#include "EnemyEntity.h"

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

#include <Game/Entities/PlayerEntity.h>

#include "Engine/SFMLMath/SFMLMath.hpp"

//using namespace std;
using namespace sf;


void EnemyEntity::Init()
{
	//Enemy
	m_pEnemySpriteComponent = AddComponent<SpriteComponent>();

	m_pEnemySpriteComponent->SetSpriteByPath("../Assets/Kenney/Dungeon/Tiles/tile_0120.png", true);
	m_pEnemySpriteComponent->GetSprite().setScale(3.f, 3.f);

	SetPosition(Vector2f(600, -100)); m_startPosition = GetPosition(); 
	m_movementSpeed = 0.1f;
	//Weapon
	/*m_pWeaponSpriteComponent = AddComponent<SpriteComponent>();
	m_pWeaponSpriteComponent->SetSpriteByPath("../Assets/Kenney/Dungeon/Tiles/d_tile_0130.png", false);
	m_pWeaponSpriteComponent->GetSprite().setScale(3.f, 3.f);
	m_pWeaponSpriteComponent->GetSprite().setPosition(m_pEnemySpriteComponent->GetSprite().getPosition() - Vector2f(15.f, -10.f));
	m_pWeaponSpriteComponent->GetSprite().setOrigin(sf::Vector2f(7.f, 14.f));*/

	//EnemyIndicator?
	/*m_pEnemyIndicatorSpriteComponent = AddComponent<SpriteComponent>();
	m_pEnemyIndicatorSpriteComponent->SetSpriteByPath("../Assets/dir_indicator.png", false);
	m_pEnemyIndicatorSpriteComponent->GetSprite().setOrigin(8.f, 63.f);*/

	//CollisionComponent
	m_pCollisionComponent = AddComponent<CircleCollisionComponent>();
	m_pCollisionComponent->SetProperties(20.f, false, true, std::bind(&EnemyEntity::OnCollide, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	//HealthbarEntity
	m_pHealthbarEntity = Engine::GetInstance()->GetEntitySystem().SpawnEntity<HealthbarEntity>();
	m_pHealthbarEntity->SetOwnerEntity(GetId());
	m_pHealthbarEntity->SetOffset(sf::Vector2f(0.f, -75.f));

	m_currentHealth = m_maxHealth;

	//Buffs
	/*std::shared_ptr<MovementSpeedBuff> pBaseMoveSpeedBuff = std::make_shared<MovementSpeedBuff>();
	pBaseMoveSpeedBuff->flatMoveSpeedIncrease = 1200.f;
	AddBuff(pBaseMoveSpeedBuff);

	std::shared_ptr<TurnSpeedBuff> pBaseTurnSpeedBuff = std::make_shared<TurnSpeedBuff>();
	pBaseTurnSpeedBuff->flatTurnSpeedIncrease = 200.f;
	AddBuff(pBaseTurnSpeedBuff);

	std::shared_ptr<BulletCountBuff> pBaseBulletCountBuff = std::make_shared<BulletCountBuff>();
	pBaseBulletCountBuff->additionalBulletsPerFire = 1;
	AddBuff(pBaseBulletCountBuff);

	ReevaluateStats();*/
}


void EnemyEntity::Update(float deltaSeconds)
{
	if (m_handleInput)
	{
		if (GetPosition() != Vector2f(0, 0)) {
			if (m_canMove)
				UpdateMovement(deltaSeconds);
		}
		else {
			m_handleInput = false;
			std::shared_ptr<GameOverGameState> pGameOverState = std::make_shared<GameOverGameState>();
			pGameOverState->loserEntityId = GetId();
			GameManager::Get()->SetGameState(pGameOverState);
		}
	}
	//UpdateWeaponRotation(deltaSeconds);

	/*if (m_fireTimer >= 0) {
		m_fireTimer -= deltaSeconds;
	}
	else m_canShoot = true;*/

	/*if (m_shooting) {
		if (m_canShoot) {
			OnInputPressed();
			m_fireTimer = m_fireRate;
			m_canShoot = false;
		}
	}*/

	UpdateEnemyIndicatorRotation(deltaSeconds);
	//UpdateBuffs(deltaSeconds);
}


void EnemyEntity::Shutdown()
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

void EnemyEntity::ProcessDamage(float damageAmount)
{
	/*for (const std::shared_ptr<Buff>& pBuff : m_buffs)
	{
		pBuff->EvaluateStat(EBuffStat::Armor, damageAmount);
	}*/

	if (damageAmount <= 0.f)
		return;

	m_currentHealth = std::max(0.f, m_currentHealth - damageAmount);

	if (m_currentHealth <= 0.f)
	{
		m_handleInput = false;
		//m_pEnemySpriteComponent->SetSpriteByPath("../Assets/Tanks/Broken_assets/tank2_broken_greyscale.png", true);
		//GameManager::Get()->GetGameState()->

		Destroy();
		//m_pWeaponSpriteComponent->SetVisibility(false);
		//IngameGameState::PlusScore(100);

		//При смерти дает очки.

		//std::shared_ptr<GameOverGameState> pGameOverState = std::make_shared<GameOverGameState>();
		//pGameOverState->loserEntityId = GetId();
		//GameManager::Get()->SetGameState(pGameOverState);
	}
}

void EnemyEntity::OnCollide(const CollisionEntry& me, const CollisionEntry& other, sf::Vector2f resolveVector)
{
	if (other.isTriggerVolume)
		return;

	if (PlayerEntity* pEntity = dynamic_cast<PlayerEntity*>(other.pEntity))
	{
		if (IDamageable* pDamageable = dynamic_cast<IDamageable*>(other.pEntity))
		{
			//if (pDamageable->GetFaction() != GetFaction())
			//{
				float damage = m_currentHealth;
				SetCanMove(false);
				pDamageable->ProcessDamage(damage);
				ProcessDamage(damage);
			//}
		}
	}
	/*/const sf::Vector2f resolveVectorNormalized = sf::getNormalized(resolveVector);
	const sf::Vector2f currentVelocity = m_pRigidbodyComponent->GetVelocity();
	const sf::Vector2f newVelocity = sf::reflect(currentVelocity, resolveVectorNormalized);

	if (std::isnan(newVelocity.x) || std::isnan(newVelocity.y))
	{
		std::cout << "nan!" << std::endl;
	}

	m_pRigidbodyComponent->SetVelocity(newVelocity);
	UpdateRotation();

	m_bounceCount++;
	if (m_maxBounces > 0 && m_bounceCount >= m_maxBounces)
	{
		Destroy();
	}*/
}

/*void EnemyEntity::SetInputKeys(sf::Mouse::Button fireButton, const TMovementKeys& movementKeys)
{
	if (m_inputCallbackId != 0)
	{
		Engine::GetInstance()->GetInputManager().Unregister(m_inputCallbackId);
		m_inputCallbackId = 0;
	}

	m_inputCallbackId = Engine::GetInstance()->GetInputManager().Register(fireButton, EInputEvent::Pressed, std::bind(&EnemyEntity::Shooting, this));
	m_inputCallbackId = Engine::GetInstance()->GetInputManager().Register(fireButton, EInputEvent::Released, std::bind(&EnemyEntity::NotShooting, this));
	m_movementKeys = movementKeys;
}*/

void EnemyEntity::SetColor(sf::Color color)
{
	m_factionColor = color;

	m_pEnemySpriteComponent->GetSprite().setColor(m_factionColor);
	//m_pWeaponSpriteComponent->GetSprite().setColor(m_factionColor);
}

void EnemyEntity::SetTargetId(int targetEntityId, sf::Color color)
{
	m_targetEntityId = targetEntityId;
	m_pEnemyIndicatorSpriteComponent->GetSprite().setColor(color);
}

void EnemyEntity::SetStartPosition() { m_startPosition = GetPosition(); }

void EnemyEntity::SetEnemyType(int type)
{
	std::string spritePath;
	switch (type)
	{
	case 0: //Bat
		spritePath = "../Assets/Kenney/Dungeon/Tiles/tile_0120.png";
		m_pEnemySpriteComponent->SetSpriteByPath(spritePath, true);
		m_maxHealth = 30;
		m_currentHealth = m_maxHealth;
		m_movementSpeed = .2f;

		break;
	case 1: //Ghost
		spritePath = "../Assets/Kenney/Dungeon/Tiles/tile_0121.png";
		m_pEnemySpriteComponent->SetSpriteByPath(spritePath, true);
		m_maxHealth = 80;
		m_currentHealth = m_maxHealth;
		m_movementSpeed = .05f;

		break;
	case 2: //Spider
		spritePath = "../Assets/Kenney/Dungeon/Tiles/tile_0122.png";
		m_pEnemySpriteComponent->SetSpriteByPath(spritePath, true);
		m_pEnemySpriteComponent->SetSpriteByPath(spritePath, true);
		m_maxHealth = 60;
		m_currentHealth = m_maxHealth;
		m_movementSpeed = .1f;


		break;
	case 3: //Rat
		spritePath = "../Assets/Kenney/Dungeon/Tiles/tile_0123.png";
		m_pEnemySpriteComponent->SetSpriteByPath(spritePath, true);
		m_maxHealth = 30;
		m_currentHealth = m_maxHealth;
		m_movementSpeed = .3f;

		break;
	}
	
	
}

/*void EnemyEntity::AddBuff(std::shared_ptr<Buff> pBuff)
{
	if (pBuff)
	{
		m_buffs.push_back(pBuff);
		ReevaluateStats();
	}
}*/

/*void EnemyEntity::UpdateMovement(sf::Vector2f movementVector, float deltaSeconds)
{
	m_mult = 1.f;

	if (std::abs(movementVector.x) + std::abs(movementVector.y) > 1) {
		m_mult /= std::sqrt(2.f);
	}

	const sf::Vector2f oldPosition = GetPosition();
	const sf::Vector2f newPosition = GetPosition() + (movementVector * deltaSeconds * m_movementSpeed * m_mult);
	SetPosition(newPosition);
	//UpdateBaseRotation(movementVector, oldPosition, newPosition);
}*/

void EnemyEntity::UpdateMovement(float deltaSeconds)
{
	m_factor += (deltaSeconds * m_movementSpeed);
	SetPosition(Interpolate(m_startPosition, Vector2f(0, 0), m_factor));
	//UpdateBaseRotation(movementVector, oldPosition, newPosition);
}

sf::Vector2f EnemyEntity::Interpolate(const sf::Vector2f& pointA, const sf::Vector2f& pointB, float factor)
{
	if (factor > 1.f)
		factor = 1.f;

	else if (factor < 0.f)
		factor = 0.f;

	return pointA + (pointB - pointA) * factor;
}

/*void EnemyEntity::UpdateBuffs(float deltaSeconds)
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
}*/

/*void EnemyEntity::ReevaluateStats()
{
	m_movementSpeed = std::min(ReevaluateStat(EBuffStat::MoveSpeed), 500.f);
	m_bulletsPerShot = std::min(static_cast<int>(ReevaluateStat(EBuffStat::BulletCount)), 5);
	m_turretTurnSpeed = std::min(ReevaluateStat(EBuffStat::TurnSpeed), 1080.f);
}*/

/*float EnemyEntity::ReevaluateStat(EBuffStat stat)
{
	float value = 0.f;
	for (std::shared_ptr<Buff> pBuff : m_buffs)
	{
		pBuff->EvaluateStat(stat, value);
	}

	return value;
}*/

sf::Vector2f EnemyEntity::GetMovementVector() const
{
	//InputManager& manager = Engine::GetInstance()->GetInputManager();

	float movementSpeed = 200.f;
	sf::Vector2f movementVector;

	return movementVector;
}



/*void EnemyEntity::UpdateBaseRotation(sf::Vector2f movementVector, sf::Vector2f oldPosition, sf::Vector2f newPosition)
{
	if (movementVector.x != 0.f || movementVector.y != 0.f)
	{
		const float degree = sf::GetRotationFromDirection(newPosition - oldPosition);
		m_pEnemySpriteComponent->GetSprite().setRotation(degree);
	}
}*/


/*void EnemyEntity::UpdateWeaponRotation(float deltaSeconds)
{
	sf::RenderWindow& renderWindow = Engine::GetInstance()->GetRenderWindow();
	sf::Vector2i mouseScreenPosition = sf::Mouse::getPosition(renderWindow);

	sf::Vector2f mouseWorldPosition = renderWindow.mapPixelToCoords(mouseScreenPosition);
	sf::Vector2f turrentPosition = GetPosition();

	const float degree = static_cast<float>(sf::radToDeg(std::atan2f(mouseWorldPosition.x - turrentPosition.x, mouseWorldPosition.y - turrentPosition.y))) * -1.f;
	m_pWeaponSpriteComponent->GetSprite().setRotation(degree+180.f);

	//const float currentRotation = m_pWeaponSpriteComponent->GetSprite().getRotation();
	//m_pWeaponSpriteComponent->GetSprite().setRotation(currentRotation + m_turretTurnSpeed * deltaSeconds);
}*/


void EnemyEntity::UpdateEnemyIndicatorRotation(float deltaSeconds)
{
	if (Entity* pTargetEntity = Engine::GetInstance()->GetEntitySystem().FindEntityById(m_targetEntityId))
	{
		m_pEnemyIndicatorSpriteComponent->GetSprite().setRotation(sf::GetRotationFromDirection(GetPosition() - pTargetEntity->GetPosition()));
	}
}

void EnemyEntity::SetCanMove(bool canMove) {m_canMove = canMove;}

/*void EnemyEntity::Shooting() { m_shooting = true;		}

void EnemyEntity::NotShooting() { m_shooting = false;		}

void EnemyEntity::OnInputPressed()
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
		BulletEntity* pBulletEntity = Engine::GetInstance()->GetEntitySystem().SpawnEntity<BulletEntity>();

		const sf::Vector2f newPosition = GetPosition() + fireVector * 61.f + (-rightVector * positionOffset) + (rightVector * bulletOffset * static_cast<float>(i));
		pBulletEntity->SetPosition(newPosition);
		pBulletEntity->SetRotation(sf::GetRotationFromDirection(newPosition - GetPosition()));
		
		pBulletEntity->Launch(500.f, 1.f, 20.f, 5, m_factionColor, GetId());
		//pBulletEntity->Launch(500.f, 1.f, 20.f, 5, Color(255, 255, 255, 255), GetId());
	}
}*/