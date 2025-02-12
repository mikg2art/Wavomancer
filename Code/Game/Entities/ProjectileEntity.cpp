#include "ProjectileEntity.h"

#include "Engine/EntitySystem/EntityFactory.h"
#include "Engine/EntitySystem/Components/SpriteComponent.h"
#include "Engine/EntitySystem/Components/RigidbodyComponent.h"
#include "Engine/EntitySystem/Components/CircleCollisionComponent.h"
#include "Engine/Engine.h"

#include <Game/Entities/EnemyEntity.h>

#include "Engine/SFMLMath/SFMLMath.hpp"

#include "Game/DamageDefinitions.h"

void ProjectileEntity::Init()
{
	m_pSpriteComponent = AddComponent<SpriteComponent>();
	//m_pSpriteComponent->SetSpriteByPath("../Assets/Kenney/Shmup/Tiles/tile_0002.png", true);
	m_pSpriteComponent->SetSpriteByPath("../Assets/Custom/tile_0003.png", true);
	m_pSpriteComponent->GetSprite().setScale(2, 2);

	m_pRigidbodyComponent = AddComponent<RigidbodyComponent>();

	m_pCollisionComponent = AddComponent<CircleCollisionComponent>();
	m_pCollisionComponent->SetProperties(10.f, false, true, std::bind(&ProjectileEntity::OnCollide, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void ProjectileEntity::Update(float deltaSeconds)
{
	m_secondsAlive += deltaSeconds;

	if (m_secondsAlive >= m_lifetime && m_lifetime > 0.f)
	{
		Destroy();
	}
}

void ProjectileEntity::Launch(float velocity, float power, float lifetime, int maxBounces, sf::Color color, int ownerId)
{
	const float rotationInDegree = GetRotation();
	const float rotationInRadians = static_cast<float>(sf::degToRad(rotationInDegree)) * -1.f;

	sf::Vector2f fireVector;
	fireVector.x = std::sin(rotationInRadians);
	fireVector.y = std::cos(rotationInRadians);

	m_pRigidbodyComponent->AddVelocity(velocity * fireVector);
	m_pSpriteComponent->GetSprite().setColor(color);

	m_ownerId = ownerId;
	m_power = power;
	m_lifetime = lifetime;
	m_maxBounces = maxBounces;
}

void ProjectileEntity::OnCollide(const CollisionEntry& me, const CollisionEntry& other, sf::Vector2f resolveVector)
{
	if (other.isTriggerVolume) {
		if (EnemyEntity* pEntity = dynamic_cast<EnemyEntity*>(other.pEntity))
		{
			if (IDamageable* pDamageable = dynamic_cast<IDamageable*>(other.pEntity))
			{
				if (pDamageable->GetFaction() != GetFaction())
				{
					pDamageable->ProcessDamage(5.f * m_power);
					Destroy();
				}
			}
		}
		else return;
	}

	if (IDamageable* pDamageable = dynamic_cast<IDamageable*>(other.pEntity))
	{
		if (pDamageable->GetFaction() != GetFaction())
		{
			pDamageable->ProcessDamage(5.f * m_power);
			Destroy();
		}
	}

	/*const sf::Vector2f resolveVectorNormalized = sf::getNormalized(resolveVector);
	const sf::Vector2f currentVelocity = m_pRigidbodyComponent->GetVelocity();
	const sf::Vector2f newVelocity = sf::reflect(currentVelocity, resolveVectorNormalized);

	if (std::isnan(newVelocity.x) || std::isnan(newVelocity.y))
	{
		//std::cout << "nan!" << std::endl;
	}

	m_pRigidbodyComponent->SetVelocity(newVelocity);
	UpdateRotation();

	m_bounceCount++;
	if (m_maxBounces > 0 && m_bounceCount >= m_maxBounces)
	{
		Destroy();
	}*/
}

void ProjectileEntity::UpdateRotation()
{
	const sf::Vector2f velocity = m_pRigidbodyComponent->GetVelocity();
	const sf::Vector2f currentPos = GetPosition();
	const sf::Vector2f targetPos = currentPos + velocity;

	SetRotation(sf::GetRotationFromDirection(targetPos - currentPos));
}
