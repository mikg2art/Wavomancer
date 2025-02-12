#include "ComponentFactory.h"

#include "Engine/EntitySystem/EntitySystemDefinitions.h"

ComponentFactoryRegistry* ComponentFactoryRegistry::s_instance = nullptr;

ComponentFactoryRegistry* ComponentFactoryRegistry::GetInstance()
{
	if (!s_instance)
		s_instance = new ComponentFactoryRegistry();

	return s_instance;
}

void ComponentFactoryRegistry::RegisterFactory(const std::type_info& typeInfo, const std::string& identifier, ComponentFactory* pFactory)
{
	m_stringToFactoryMap[identifier] = pFactory;
	m_typeIdToStringMap[&typeInfo] = identifier;
}

EntityComponent* ComponentFactoryRegistry::CreateComponent(const std::string& identifier) const
{
	StringToFactoryMap::const_iterator it = m_stringToFactoryMap.find(identifier);
	if (it != m_stringToFactoryMap.end())
	{
		return it->second->Create();
	}

	return nullptr;
}

std::string ComponentFactoryRegistry::GetIdentifierFromComponent(const EntityComponent& component) const
{
	const std::type_info& myTypeId = typeid(component);
	TypeIdToStringMap::const_iterator it = m_typeIdToStringMap.find(&myTypeId);
	if (it != m_typeIdToStringMap.end())
	{
		return it->second;
	}

	return "<Unknown>";
}
