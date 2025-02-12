#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

class Entity;

class EntityComponent
{
public:

	void Init(Entity* pOwner);
	virtual ~EntityComponent() = default;

	virtual void Start() {}
	virtual void Update(float deltaSeconds) {}
	virtual void Shutdown() {}

	virtual void OnPositionChanged() {}

	Entity* GetOwner() const { return m_pOwner; }

protected:

	Entity* m_pOwner = nullptr;

};

class Entity
{
public:

	virtual ~Entity() = default;

	void EntityInit(const std::string& name, int id);
	void EntityUpdate(float deltaSeconds);
	void EntityShutdown();

	void Destroy();
	bool IsMarkedForDestruction() const { return m_isMarkedForDestruction; }

	template<typename T>
	T* AddComponent();

	template<typename T>
	void RemoveComponent();
	void RemoveComponent(EntityComponent& componentToRemove);

	template<typename T>
	T* GetComponent();

	void SetPosition(sf::Vector2f newPosition);
	sf::Vector2f GetPosition() const;
	
	void SetRotation(float newRotation);
	float GetRotation() const;

	void SetScale(sf::Vector2f newScale);
	sf::Vector2f GetScale() const;

	const sf::Transform& GetTransform();

	int GetId() const { return m_id; }

protected:

	virtual void Init() { }
	virtual void Update(float deltaSeconds) { }
	virtual void Shutdown() { }

	virtual void OnPositionChanged() { }

private:

	void RemoveComponent_Internal(EntityComponent& componentToRemove);

	std::string m_name;
	std::vector<EntityComponent*> m_components;
	bool m_isMarkedForDestruction = false;
	int m_id = 0;

	sf::Vector2f m_position;
	float m_rotation = 0.f;
	sf::Vector2f m_scale = sf::Vector2f(1.f, 1.f);

	sf::Transform m_transform;
};

template<typename T>
inline T* Entity::AddComponent()
{
	T* pNewComponent = new T();
	pNewComponent->Init(this);
	m_components.push_back(pNewComponent);

	pNewComponent->Start();

	return pNewComponent;
}

template<typename T>
inline void Entity::RemoveComponent()
{
	for (EntityComponent* myComp : m_components)
	{
		RenderComponent* myCastedComponent = dynamic_cast<T*>(myComp);
		if (myCastedComponent)
		{
			RemoveComponent_Internal(*myCastedComponent);
		}
	}
}

template<typename T>
inline T* Entity::GetComponent()
{
	for (EntityComponent* myComp : m_components)
	{
		T* myCastedComponent = dynamic_cast<T*>(myComp);
		if (myCastedComponent)
		{
			return myCastedComponent;
		}
	}

	return nullptr;
}
