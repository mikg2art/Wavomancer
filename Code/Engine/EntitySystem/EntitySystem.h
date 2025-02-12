#pragma once

#include "EntitySystemDefinitions.h"

#include "Engine/EntitySystem/ComponentFactory.h"

class EntitySystem
{
public:

	template<typename T>
	T* SpawnEntity();

	void DestroyAllEntities();

	template<typename T>
	void DestroyAllEntitiesOfType();
	template<typename T>
	size_t CountAllEntitiesOfType();

	void CleanUpDestroyedEntities();

	void Update(float deltaSeconds);
	size_t GetEntityCount() const { return m_entities.size(); }

	Entity* FindEntityById(int id) const;

private:

	std::vector<Entity*> m_entities;
	int m_idCounter = 0;

};

template<typename T>
inline T* EntitySystem::SpawnEntity()
{
	T* pNewEntity = new T();
	m_entities.push_back(pNewEntity);
	pNewEntity->EntityInit("Entity" + std::to_string(m_entities.size()), ++m_idCounter);

	return pNewEntity;
}

template<typename T>
void EntitySystem::DestroyAllEntitiesOfType()
{
	for (Entity* pEntity : m_entities)
	{
		if (T* pValidEntity = dynamic_cast<T*>(pEntity))
		{
			pValidEntity->Destroy();
		}
	}
}

template<typename T>
size_t EntitySystem::CountAllEntitiesOfType()
{
	size_t count = 0;
	for (Entity* pEntity : m_entities)
	{
		if (T* pValidEntity = dynamic_cast<T*>(pEntity))
		{
			count++;
		}
	}

	return count;
}
