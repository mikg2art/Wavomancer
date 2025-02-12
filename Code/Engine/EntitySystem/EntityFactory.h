#pragma once

#include <map>
#include <string>
#include <typeinfo>

#include "Engine/EntitySystem/EntitySystem.h"

class Entity;

class EntityFactory
{
public:
	virtual Entity* Create() = 0;
};

template<typename EntityType>
class EntityFactory_Impl : public EntityFactory
{
public:
	EntityFactory_Impl(const std::string& identifier)
		: m_identifier(identifier)
	{
		EntityFactoryRegistry::GetInstance()->RegisterFactory(typeid(EntityType), m_identifier, this);
	}

	virtual Entity* Create() override { return Engine::GetInstance()->GetEntitySystem().SpawnEntity<EntityType>(); }

	std::string m_identifier;
};

class EntityFactoryRegistry
{
	static EntityFactoryRegistry* s_instance;

public:

	static EntityFactoryRegistry* GetInstance();

	void RegisterFactory(const std::type_info& typeInfo, const std::string& identifier, EntityFactory* pFactory);
	Entity* CreateEntity(const std::string& identifier) const;
	std::string GetIdentifierFromEntity(const Entity& entity) const;

private:

	using StringToFactoryMap = std::map<std::string, EntityFactory*>;
	using TypeIdToStringMap = std::map<const std::type_info*, std::string>;

	StringToFactoryMap m_stringToFactoryMap;
	TypeIdToStringMap m_typeIdToStringMap;
};

#define REGISTER_ENTITY_FACTORY(type) EntityFactory_Impl<type> type##Factory(#type);
