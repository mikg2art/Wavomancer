#include "EntitySystem.h"

void EntitySystem::DestroyAllEntities()
{
	for (Entity* pEntity : m_entities)
	{
		pEntity->Destroy();
	}
}

void EntitySystem::CleanUpDestroyedEntities()
{
	for (int i = static_cast<int>(m_entities.size()) - 1; i >= 0; i--)
	{
		Entity* pEntity = m_entities[i];

		if (pEntity->IsMarkedForDestruction())
		{
			m_entities.erase(m_entities.begin() + i);
			delete pEntity;
		}
	}
}

void EntitySystem::Update(float deltaSeconds)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		Entity* pEntity = m_entities[i];
		pEntity->EntityUpdate(deltaSeconds);
	}

	CleanUpDestroyedEntities();
}

Entity* EntitySystem::FindEntityById(int id) const
{
	for (Entity* pEntity : m_entities)
	{
		if (pEntity->GetId() == id)
		{
			return pEntity;
		}
	}

	return nullptr;
}
