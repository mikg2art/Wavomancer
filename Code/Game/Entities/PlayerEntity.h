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
class ViewComponent;
class HealthbarEntity;

class PlayerEntity : public Entity, public IDamageable
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

	void SetInputKeys(sf::Mouse::Button fireButton, const TMovementKeys& movementKeys);
	void SetColor(sf::Color color);
	void SetTargetId(int targetEntityId, sf::Color color);

	void SetCanMove(bool canMove);
	void SetFireRate(float fireRate);

	void AddBuff(std::shared_ptr<Buff> pBuff);

protected:

	void UpdateMovement(sf::Vector2f movementVector, float deltaSeconds);
	void UpdateBuffs(float deltaSeconds);
	
	void ReevaluateStats();
	float ReevaluateStat(EBuffStat stat);

	sf::Vector2f GetMovementVector() const;
	//void UpdateBaseRotation(sf::Vector2f movementVector, sf::Vector2f oldPosition, sf::Vector2f newPosition);
	void UpdateWeaponRotation(float deltaSeconds);
	void UpdateEnemyIndicatorRotation(float deltaSeconds);

	void Shooting();
	void NotShooting();

	void OnInputPressed();

	int m_inputCallbackId = 0;
	int m_targetEntityId = 0;

	SpriteComponent* m_pPlayerSpriteComponent = nullptr;
	SpriteComponent* m_pWeaponSpriteComponent = nullptr;
	SpriteComponent* m_pEnemyIndicatorSpriteComponent = nullptr;
	CircleCollisionComponent* m_pCollisionComponent = nullptr;
	HealthbarEntity* m_pHealthbarEntity = nullptr;

	TMovementKeys m_movementKeys;

	float m_movementSpeed = 0.f;
	float m_turretTurnSpeed = 0.f;
	bool m_canMove = true;

	float m_mult = 1.f;

	bool m_shooting = false;
	bool m_canShoot = false;
	float m_fireRate = 0.2f;
	float m_fireTimer = 0.f;

	float m_currentHealth = 0.f;
	float m_maxHealth = 100.f;

	int m_bulletsPerShot = 0;

	bool m_handleInput = true;

	sf::Color m_factionColor;
	std::vector<std::shared_ptr<Buff>> m_buffs;
};