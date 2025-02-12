#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"
#include "Engine/Input/InputManager.h"

#include "Game/DamageDefinitions.h"

class SpriteComponent;
class RigidbodyComponent;
class CircleCollisionComponent;
struct CollisionEntry;

class ProjectileEntity : public Entity, public IDamageable
{
public:

	//Entity
	virtual void Init() override;
	virtual void Update(float deltaSeconds) override;
	//~Entity

	//IDamageable
	virtual void ProcessDamage(float damageAmount) override { Destroy(); }
	virtual int GetFaction() override { return m_ownerId; }
	//~IDamageable

	void Launch(float velocity, float power, float lifetime, int maxBounces, sf::Color color, int ownerId);
	int GetOwnerId() const { return m_ownerId; }

protected:

	void OnCollide(const CollisionEntry& me, const CollisionEntry& other, sf::Vector2f resolveVector);
	void UpdateRotation();

	SpriteComponent* m_pSpriteComponent = nullptr;
	RigidbodyComponent* m_pRigidbodyComponent = nullptr;
	CircleCollisionComponent* m_pCollisionComponent = nullptr;

	int m_ownerId = 0;
	float m_power = 0.f;
	float m_secondsAlive = 0.f;
	float m_lifetime = 0.f;
	int m_maxBounces = 0;
	int m_bounceCount = 0;

};