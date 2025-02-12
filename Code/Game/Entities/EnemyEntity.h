#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"
#include "Game/DamageDefinitions.h"
#include "Engine/Input/InputManager.h"

#include <SFML/Graphics.hpp>
#include <array>
#include <Game/BuffDefinitions.h>

class SpriteComponent;
class RigidbodyComponent;
class CircleCollisionComponent;
//class ViewComponent;
class HealthbarEntity;
struct CollisionEntry;

enum class EEnemyType 
{
	Rat,
	Ghost,
	Spider,
	Bat
};

class EnemyEntity : public Entity, public IDamageable
{
	using TMovementKeys = std::array<sf::Keyboard::Key, 4>;

public:

	// Entity
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void Update(float deltaSeconds) override;
	// ~Entity

	// IDamageable
	virtual void ProcessDamage(float damageAmount) override;
	virtual float GetHealth() const override { return m_currentHealth; }
	virtual float GetMaxHealth() const override { return m_maxHealth; }
	// ~IDamageable

	//void SetInputKeys(sf::Mouse::Button fireButton, const TMovementKeys& movementKeys);
	void SetColor(sf::Color color);
	void SetTargetId(int targetEntityId, sf::Color color);
	void SetStartPosition();
	void SetEnemyType(int type);

	void AddBuff(std::shared_ptr<Buff> pBuff);

protected:

	void OnCollide(const CollisionEntry& me, const CollisionEntry& other, sf::Vector2f resolveVector);
	void UpdateMovement(float deltaSeconds);
	void UpdateBuffs(float deltaSeconds);
	
	void ReevaluateStats();
	float ReevaluateStat(EBuffStat stat);

	sf::Vector2f Interpolate(const sf::Vector2f& pointA, const sf::Vector2f& pointB, float factor);
	sf::Vector2f GetMovementVector() const;
	
	//void UpdateBaseRotation(sf::Vector2f movementVector, sf::Vector2f oldPosition, sf::Vector2f newPosition);
	//void UpdateWeaponRotation(float deltaSeconds);
	void UpdateEnemyIndicatorRotation(float deltaSeconds);

	void SetCanMove(bool canMove);

	//void Shooting();
	//void NotShooting();

	//void OnInputPressed();

	int m_inputCallbackId = 0;
	int m_targetEntityId = 0;

	SpriteComponent* m_pEnemySpriteComponent = nullptr;
	SpriteComponent* m_pWeaponSpriteComponent = nullptr;
	SpriteComponent* m_pEnemyIndicatorSpriteComponent = nullptr;
	CircleCollisionComponent* m_pCollisionComponent = nullptr;
	HealthbarEntity* m_pHealthbarEntity = nullptr;

	TMovementKeys m_movementKeys;
	sf::Vector2f m_startPosition;
	float m_factor = 0;
	float m_movementSpeed = .2f;

	bool m_canMove = true;
	/*
	float m_movementSpeed = 0.f;
	float m_turretTurnSpeed = 0.f;

	float m_mult = 1.f;

	bool m_shooting = false;
	bool m_canShoot = false;
	float m_fireRate = 0.2f;
	float m_fireTimer = 0.f;*/

	EEnemyType m_class = EEnemyType::Bat;

	float m_currentHealth = 0.f;
	float m_maxHealth = 75.f;

	//int m_bulletsPerShot = 0;

	bool m_handleInput = true;

	sf::Color m_factionColor;
	std::vector<std::shared_ptr<Buff>> m_buffs;
};