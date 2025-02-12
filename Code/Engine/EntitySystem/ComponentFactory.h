#pragma once

#include <map>
#include <string>
#include <typeinfo>

class EntityComponent;

class ComponentFactory
{
public:
	virtual EntityComponent* Create() = 0;
};

template<typename ComponentType>
class ComponentFactory_Impl : public ComponentFactory
{
public:
	ComponentFactory_Impl(const std::string& identifier)
		: m_identifier(identifier)
	{
		ComponentFactoryRegistry::GetInstance()->RegisterFactory(typeid(ComponentType), m_identifier, this);
	}

	virtual EntityComponent* Create() override { return new ComponentType(); }

	std::string m_identifier;
};

class ComponentFactoryRegistry
{
	static ComponentFactoryRegistry* s_instance;

public:

	static ComponentFactoryRegistry* GetInstance();

	void RegisterFactory(const std::type_info& typeInfo, const std::string& identifier, ComponentFactory* pFactory);
	EntityComponent* CreateComponent(const std::string& identifier) const;
	std::string GetIdentifierFromComponent(const EntityComponent& component) const;

private:

	using StringToFactoryMap = std::map<std::string, ComponentFactory*>;
	using TypeIdToStringMap = std::map<const std::type_info*, std::string>;

	StringToFactoryMap m_stringToFactoryMap;
	TypeIdToStringMap m_typeIdToStringMap;
};

#define REGISTER_COMPONENT_FACTORY(type) ComponentFactory_Impl<type> type##Factory(#type);
