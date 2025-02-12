#include "RigidbodyComponent.h"
#include "Engine/Engine.h"

#include "Engine/EntitySystem/ComponentFactory.h"

void RigidbodyComponent::Start()
{
}

void RigidbodyComponent::Update(float deltaSeconds)
{
	//apply drag to velocity
	sf::Vector2f currentPos = GetOwner()->GetPosition();
	currentPos += m_velocity * deltaSeconds;
	GetOwner()->SetPosition(currentPos);
}

void RigidbodyComponent::AddVelocity(sf::Vector2f velocity)
{
	m_velocity += velocity;
}

void RigidbodyComponent::SetVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

REGISTER_COMPONENT_FACTORY(RigidbodyComponent)