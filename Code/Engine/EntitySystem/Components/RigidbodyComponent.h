#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"

class RigidbodyComponent : public EntityComponent
{
public:

	virtual void Start() override;
	virtual void Update(float deltaSeconds) override;

	void AddVelocity(sf::Vector2f velocity);
	void SetVelocity(sf::Vector2f velocity);
	sf::Vector2f GetVelocity() const { return m_velocity; }

protected:

	sf::Vector2f m_velocity;
	float m_drag;

};