#include "BoxCollisionComponent.h"
#include "Engine/Engine.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Engine/EntitySystem/ComponentFactory.h"

#include <functional>

void BoxCollisionComponent::Shutdown()
{
	if (m_collisionEntryId != 0)
	{
		Engine::GetInstance()->GetCollisionManager().UnregisterShape(m_collisionEntryId);
	}
}

void BoxCollisionComponent::OnPositionChanged()
{
	Engine::GetInstance()->GetCollisionManager().UpdateShapePosition(m_collisionEntryId, GetOwner()->GetPosition());
}

void BoxCollisionComponent::SetProperties(float width, float height, bool isStatic, bool isTriggerVolume, TCollisionCallbackSignature callback)
{
	if (m_collisionEntryId != 0)
	{
		Engine::GetInstance()->GetCollisionManager().UnregisterShape(m_collisionEntryId);
	}

	CollisionShape shape;
	shape.type = EShapeType::Box;
	shape.width = width;
	shape.height = height;

	m_collisionEntryId = Engine::GetInstance()->GetCollisionManager().RegisterShape(GetOwner(), shape, GetOwner()->GetPosition(), isStatic, isTriggerVolume, callback);
}

REGISTER_COMPONENT_FACTORY(BoxCollisionComponent)
