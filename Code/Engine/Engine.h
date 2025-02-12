#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class InputManager;
class AssetRegistry;
class EntitySystem;
class CollisionManager;
class RenderSystem;

namespace sf
{
	class RenderWindow;
}

struct EngineStartUpParams
{
	std::string windowName;
	sf::Vector2u windowSize;
	bool isWindowResizable = true;
};

class Engine
{

public:
	static Engine* s_instance;
	static Engine* GetInstance() { return s_instance; }

public:

	~Engine();

	void Run(const EngineStartUpParams& params);

	sf::RenderWindow& GetRenderWindow() { return *m_pRenderWindow; }
	InputManager& GetInputManager() { return *m_pInputManager; }
	AssetRegistry& GetAssetRegistry() { return *m_pAssetRegistry; }
	EntitySystem& GetEntitySystem() { return *m_pEntitySystem; }
	CollisionManager& GetCollisionManager() { return *m_pCollisionManager; }
	RenderSystem& GetRenderSystem() { return *m_pRenderSystem; }

	sf::Time GetElapsedTime() const { return m_clock.getElapsedTime(); }

private:

	void SaveToFile();

	std::unique_ptr<sf::RenderWindow> m_pRenderWindow;
	std::unique_ptr<InputManager> m_pInputManager;
	std::unique_ptr<EntitySystem> m_pEntitySystem;
	std::unique_ptr<AssetRegistry> m_pAssetRegistry;
	std::unique_ptr<CollisionManager> m_pCollisionManager;
	std::unique_ptr<RenderSystem> m_pRenderSystem;

	sf::Clock m_clock;
	sf::Time m_lastFrameTime;

};
