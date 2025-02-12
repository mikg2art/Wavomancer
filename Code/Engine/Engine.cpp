#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "EntitySystem/EntitySystemDefinitions.h"
#include "EntitySystem/EntitySystem.h"

#include "EntitySystem/Components/SpriteComponent.h"

#include "Engine/EntitySystem/EntityFactory.h"
#include "Engine/EntitySystem/ComponentFactory.h"

#include "Engine/Input/InputManager.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/EntitySystem/EntitySystem.h"
#include "Engine/Physics/CollisionManager.h"
#include "Engine/Rendering/RenderSystem.h"

#include "Engine.h"

#include <functional>
#include <fstream>
#include <nlohmann/json.hpp>

Engine* Engine::s_instance = nullptr;

Engine::~Engine()
{

}

void Engine::Run(const EngineStartUpParams& params)
{
	// Singleton logic
	s_instance = this;

	// Sub-system creation
	sf::Uint32 style = 
		sf::Style::Close |
		sf::Style::Titlebar |
		(params.isWindowResizable ? sf::Style::Resize : 0);

	m_pRenderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(params.windowSize.x, params.windowSize.y), params.windowName, style);
	m_pInputManager = std::make_unique<InputManager>();
	m_pAssetRegistry = std::make_unique<AssetRegistry>();
	m_pEntitySystem = std::make_unique<EntitySystem>();
	m_pCollisionManager = std::make_unique<CollisionManager>();
	m_pRenderSystem = std::make_unique<RenderSystem>();

	// Loop setup
	m_pRenderWindow->setActive(true);
	int frameCount = 0;

	float fpsCounter = 0.f;
	int frameCountAtLastSecondStep = 0;

	int currentFPS = 0;

	// Level load phase
	std::ifstream configFileStream("../Assets/Game.cfg");
	nlohmann::json configFileJson = nlohmann::json::parse(configFileStream);
	const std::string defaultEntityString = configFileJson["DefaultEntity"].get<std::string>();

	Entity* pDefaultEntity = EntityFactoryRegistry::GetInstance()->CreateEntity(defaultEntityString);
	if (!pDefaultEntity)
	{
		std::cout << "Could not load default entity! Game will shut down";
		m_pRenderWindow->close();
		return;
	}

	while (m_pRenderWindow->isOpen())
	{
		sf::Time timeAtFrameStart = m_clock.getElapsedTime();
		float deltaSeconds = (timeAtFrameStart - m_lastFrameTime).asSeconds();

		fpsCounter += deltaSeconds;
		if (fpsCounter >= 1.f)
		{
			fpsCounter -= 1.f;
			currentFPS = frameCount - frameCountAtLastSecondStep;
			frameCountAtLastSecondStep = frameCount;
		}

		m_pRenderWindow->clear(sf::Color(15, 15, 15));

		// Game Loop
		frameCount++;

		// Message Pump
		sf::Event event;
		while (m_pRenderWindow->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::GainedFocus:
			{
				std::cout << "Gained Focus" << std::endl;
				break;
			}
			case sf::Event::Closed:
			{
				std::cout << "Closed" << std::endl;
				m_pRenderWindow->close();
				break;
			}
			case sf::Event::Resized:
			{
				std::cout << "Resized" << std::endl;
				break;
			}
			}
		}

		/*std::cout << "Frame " << frameCount << std::endl;
		std::cout << "DeltaSeconds: " << deltaSeconds << std::endl;
		std::cout << "FPS: " << currentFPS << std::endl;
		std::cout << "_________________" << std::endl;*/


		//////////////////////////////////////////////////////////////////////////


		// Update/Tick on all entities/actors/gameObject

		m_pInputManager->Update();
		m_pCollisionManager->Update(deltaSeconds);

		m_pEntitySystem->Update(deltaSeconds);

		m_pRenderSystem->Update();

		//////////////////////////////////////////////////////////////////////////
		m_pRenderWindow->display();

		m_lastFrameTime = timeAtFrameStart;
	}
}

void Engine::SaveToFile()
{
	/*
	Entity ent;

	if (SpriteComponent* pSpriteComponent = ent.GetComponent<SpriteComponent>())
	{
		pSpriteComponent->GetSprite()
	}
	if (CharacterControllerComponent* pComp = ...)
	*/
}
