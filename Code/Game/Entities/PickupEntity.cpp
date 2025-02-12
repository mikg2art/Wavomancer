#include "PickupEntity.h"

#include "Engine/EntitySystem/Components/SpriteComponent.h"
#include "Engine/EntitySystem/Components/CircleCollisionComponent.h"
#include "Engine/Engine.h"
#include <Game/Entities/PlayerEntity.h>

void PickUpEntity::Init()
{
	m_pSpriteComponent = AddComponent<SpriteComponent>();

	m_pCollisionComponent = AddComponent<CircleCollisionComponent>();
	m_pCollisionComponent->SetProperties(
		15.f,
		true,
		true,
		std::bind(
			&PickUpEntity::OnOverlap,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3)
	);
}

void PickUpEntity::SetBuff(std::shared_ptr<Buff> pBuff)
{
	m_pBuffToApply = pBuff;
}

void PickUpEntity::OnOverlap(const CollisionEntry& me, const CollisionEntry& other, sf::Vector2f resolveVector)
{
	if (PlayerEntity* pEntity = dynamic_cast<PlayerEntity*>(other.pEntity))
	{
		pEntity->AddBuff(m_pBuffToApply);
		Destroy();
	}
}

void MoveSpeedPickUp::Init()
{
	PickUpEntity::Init();

	std::shared_ptr<MovementSpeedBuff> pBuff = std::make_shared<MovementSpeedBuff>();
	pBuff->lifetime = 5.f;
	pBuff->multiplierMoveSpeedIncrease = 1.5f;

	SetBuff(pBuff);

	m_pSpriteComponent->SetSpriteByPath("../Assets/Kenney/Dungeon/Tiles/tile_0116.png", true);
	m_pSpriteComponent->GetSprite().setScale(3, 3);
}

void TripleShotPickUp::Init()
{
	PickUpEntity::Init();

	std::shared_ptr<BulletCountBuff> pBuff = std::make_shared<BulletCountBuff>();
	pBuff->lifetime = 5.f;
	pBuff->additionalBulletsPerFire = 2;
	SetBuff(pBuff);

	m_pSpriteComponent->SetSpriteByPath("../Assets/Kenney/Dungeon/Tiles/tile_0114.png", true);
	m_pSpriteComponent->GetSprite().setScale(3, 3);
}
