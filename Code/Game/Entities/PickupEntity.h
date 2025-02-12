#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"
#include <SFML/Graphics.hpp>
#include <Game/BuffDefinitions.h>

class SpriteComponent;
class CircleCollisionComponent;
struct CollisionEntry;

class PickUpEntity : public Entity
{

public:

	// Entity
	virtual void Init() override;
	// ~Entity

	void SetBuff(std::shared_ptr<Buff> pBuff);

protected:

	void OnOverlap(const CollisionEntry& me, const CollisionEntry& other, sf::Vector2f resolveVector);

	std::shared_ptr<Buff> m_pBuffToApply;
	SpriteComponent* m_pSpriteComponent = nullptr;
	CircleCollisionComponent* m_pCollisionComponent = nullptr;

};

class MoveSpeedPickUp : public PickUpEntity
{
	virtual void Init() override;
};

class TripleShotPickUp : public PickUpEntity
{
	virtual void Init() override;
};