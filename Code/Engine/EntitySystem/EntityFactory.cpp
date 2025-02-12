#include "EntityFactory.h"

#include "Engine/EntitySystem/EntitySystemDefinitions.h"

EntityFactoryRegistry* EntityFactoryRegistry::s_instance = nullptr;

EntityFactoryRegistry* EntityFactoryRegistry::GetInstance()
{
	if (!s_instance)
		s_instance = new EntityFactoryRegistry();

	return s_instance;
}

void EntityFactoryRegistry::RegisterFactory(const std::type_info& typeInfo, const std::string& identifier, EntityFactory* pFactory)
{
	m_stringToFactoryMap[identifier] = pFactory;
	m_typeIdToStringMap[&typeInfo] = identifier;
}

Entity* EntityFactoryRegistry::CreateEntity(const std::string& identifier) const
{
	StringToFactoryMap::const_iterator it = m_stringToFactoryMap.find(identifier);
	if (it != m_stringToFactoryMap.end())
	{
		return it->second->Create();
	}

	return nullptr;
}

std::string EntityFactoryRegistry::GetIdentifierFromEntity(const Entity& entity) const
{
	const std::type_info& myTypeId = typeid(entity);
	TypeIdToStringMap::const_iterator it = m_typeIdToStringMap.find(&myTypeId);
	if (it != m_typeIdToStringMap.end())
	{
		return it->second;
	}

	return "<Unknown>";
}