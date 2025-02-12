#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"
#include <Game/DamageDefinitions.h>

class SpriteComponent;

class HealthbarEntity : public Entity
{
public:

	virtual void Init() override;
	virtual void Update(float deltaSeconds) override;

	void SetOwnerEntity(int ownerId);
	void SetOffset(sf::Vector2f offset);

private:

	void UpdateHealthColor();

	int m_ownerId = 0;
	sf::Vector2f m_offset;

	SpriteComponent* m_pBackground = nullptr;
	SpriteComponent* m_pHealthFill = nullptr;

};