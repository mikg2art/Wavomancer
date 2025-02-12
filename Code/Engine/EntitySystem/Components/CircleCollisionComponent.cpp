#include "CircleCollisionComponent.h"
#include "Engine/Engine.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Engine/EntitySystem/ComponentFactory.h"

#include <functional>

void CircleCollisionComponent::Shutdown()
{
	if (m_collisionEntryId != 0)
	{
		Engine::GetInstance()->GetCollisionManager().UnregisterShape(m_collisionEntryId);
	}
}

void CircleCollisionComponent::OnPositionChanged()
{
	Engine::GetInstance()->GetCollisionManager().UpdateShapePosition(m_collisionEntryId, GetOwner()->GetPosition());
}

void CircleCollisionComponent::SetProperties(float radius, bool isStatic, bool isTriggerVolume, TCollisionCallbackSignature callback)
{
	if (m_collisionEntryId != 0)
	{
		Engine::GetInstance()->GetCollisionManager().UnregisterShape(m_collisionEntryId);
	}

	CollisionShape shape;
	shape.type = EShapeType::Circle;
	shape.radius = radius;

	m_collisionEntryId = Engine::GetInstance()->GetCollisionManager().RegisterShape(GetOwner(), shape, GetOwner()->GetPosition(), isStatic, isTriggerVolume, callback);
}

REGISTER_COMPONENT_FACTORY(CircleCollisionComponent)