#include "Engine/Engine.h"

#include "Engine/EntitySystem/EntityFactory.h"
#include "Game/Entities/PlayerEntity.h"
#include "Game/Entities/GameManager.h"

#include "Engine/Assets/AssetRegistry.h"

#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Physics/CollisionManager.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include <typeinfo>
#include <array>

REGISTER_ENTITY_FACTORY(GameManager)

int main()
{
	EngineStartUpParams params;
	params.windowName = "Wavomancer";
	params.windowSize = sf::Vector2u(1600, 900);
	params.isWindowResizable = false;

	std::unique_ptr<Engine> pEngine = std::make_unique<Engine>();
	pEngine->Run(params);

	return 0;
}