#include "EntitySystemDefinitions.h"

#include "Engine/Engine.h"
#include "Engine/EntitySystem/EntitySystem.h"

void EntityComponent::Init(Entity* pOwner)
{
	m_pOwner = pOwner;
}

void Entity::EntityInit(const std::string& name, int id)
{
	m_name = name;
	m_id = id;

	Init();
}

void Entity::EntityUpdate(float deltaSeconds)
{
	if (IsMarkedForDestruction())
		return;

	Update(deltaSeconds);

	for (EntityComponent* pComponent : m_components)
	{
		pComponent->Update(deltaSeconds);
	}
}

void Entity::EntityShutdown()
{
	Shutdown();

	for (EntityComponent* pComponent : m_components)
	{
		pComponent->Shutdown();
	}
}

void Entity::Destroy()
{
	EntityShutdown();
	m_isMarkedForDestruction = true;
}

void Entity::RemoveComponent(EntityComponent& componentToRemove)
{
	if (componentToRemove.GetOwner() == this)
	{
		RemoveComponent_Internal(componentToRemove);
	}
}

void Entity::SetPosition(sf::Vector2f newPosition)
{
	if (m_position != newPosition)
	{
		m_position = newPosition;
	
		OnPositionChanged();
		for (EntityComponent* pComponent : m_components)
		{
			pComponent->OnPositionChanged();
		}
	}
}

sf::Vector2f Entity::GetPosition() const
{
	return m_position;
}

void Entity::SetRotation(float newRotation)
{
	m_rotation = newRotation;
}

float Entity::GetRotation() const
{
	return m_rotation;
}

void Entity::SetScale(sf::Vector2f newScale)
{
	m_scale = newScale;
}

sf::Vector2f Entity::GetScale() const
{
	return m_scale;
}

const sf::Transform& Entity::GetTransform()
{
	m_transform = sf::Transform::Identity;
	m_transform.translate(m_position).rotate(m_rotation).scale(m_scale);

	return m_transform;
}

void Entity::RemoveComponent_Internal(EntityComponent& componentToRemove)
{
	componentToRemove.Shutdown();
	
	for (int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i] == &componentToRemove)
		{
			m_components.erase(m_components.begin() + i);
		}
	}
}
