#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"
#include "Engine/Physics/CollisionManager.h"

class CircleCollisionComponent : public EntityComponent
{
public:

	virtual void Shutdown() override;
	virtual void OnPositionChanged() override;

	void SetProperties(float radius, bool isStatic, bool isTriggerVolume, TCollisionCallbackSignature callback);

private:

	int m_collisionEntryId;

};