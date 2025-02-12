cmake_minimum_required(VERSION 3.1)

set(ENGINE_PROJECT_NAME "Engine")
set(GAME_PROJECT_NAME "Game")
set(CMAKE_CXX_STANDARD 17)

IF (WIN32)
	set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/Code/ThirdParty/SFML/lib/cmake/SFML")
ELSE()
	set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/Code/ThirdParty/SFML_GCC/lib/cmake/SFML")
ENDIF()

set(SFML_STATIC_LIBRARIES TRUE)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(nlohmann_json_subDir "Code/ThirdParty/json-3.9.1/")
add_subdirectory(${nlohmann_json_subDir})

set(tinyxml2_subDir "Code/ThirdParty/tinyxml2/")
add_subdirectory(${tinyxml2_subDir})

project(${ENGINE_PROJECT_NAME})
project(${GAME_PROJECT_NAME})

find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/Code)

set(ENGINE_SOURCE_PATH Code/${ENGINE_PROJECT_NAME})
set(ENGINE_SOURCE_FILES 
${ENGINE_SOURCE_PATH}/Engine.cpp
${ENGINE_SOURCE_PATH}/Engine.h
${ENGINE_SOURCE_PATH}/SFMLMath/SFMLMath.hpp
${ENGINE_SOURCE_PATH}/EntitySystem/EntitySystemDefinitions.h
${ENGINE_SOURCE_PATH}/SystemCore.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/EntitySystemDefinitions.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/EntitySystem.h
${ENGINE_SOURCE_PATH}/EntitySystem/EntitySystem.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/Components/SpriteComponent.h
${ENGINE_SOURCE_PATH}/EntitySystem/Components/SpriteComponent.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/Components/TextRenderComponent.h
${ENGINE_SOURCE_PATH}/EntitySystem/Components/TextRenderComponent.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/Components/RigidbodyComponent.h
${ENGINE_SOURCE_PATH}/EntitySystem/Components/RigidbodyComponent.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/Components/CircleCollisionComponent.h
${ENGINE_SOURCE_PATH}/EntitySystem/Components/CircleCollisionComponent.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/Components/BoxCollisionComponent.h
${ENGINE_SOURCE_PATH}/EntitySystem/Components/BoxCollisionComponent.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/Components/ViewComponent.h
${ENGINE_SOURCE_PATH}/EntitySystem/Components/ViewComponent.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/ComponentFactory.h
${ENGINE_SOURCE_PATH}/EntitySystem/ComponentFactory.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/EntityFactory.h
${ENGINE_SOURCE_PATH}/EntitySystem/EntityFactory.cpp
${ENGINE_SOURCE_PATH}/Input/InputSystemDefinitions.h
${ENGINE_SOURCE_PATH}/Input/InputSystemDefinitions.cpp
${ENGINE_SOURCE_PATH}/Input/InputManager.cpp
${ENGINE_SOURCE_PATH}/Input/InputManager.h
${ENGINE_SOURCE_PATH}/Assets/AssetRegistry.cpp
${ENGINE_SOURCE_PATH}/Assets/AssetRegistry.h
${ENGINE_SOURCE_PATH}/Physics/CollisionManager.cpp
${ENGINE_SOURCE_PATH}/Physics/CollisionManager.h
${ENGINE_SOURCE_PATH}/Rendering/RenderSystem.cpp
${ENGINE_SOURCE_PATH}/Rendering/RenderSystem.h
${ENGINE_SOURCE_PATH}/SystemCore.h
)

set(GAME_SOURCE_PATH Code/${GAME_PROJECT_NAME})
set(GAME_SOURCE_FILES 
${GAME_SOURCE_PATH}/Game.cpp
${GAME_SOURCE_PATH}/DamageDefinitions.cpp
${GAME_SOURCE_PATH}/DamageDefinitions.h
${GAME_SOURCE_PATH}/BuffDefinitions.cpp
${GAME_SOURCE_PATH}/BuffDefinitions.h
${GAME_SOURCE_PATH}/GameStates.cpp
${GAME_SOURCE_PATH}/GameStates.h
${GAME_SOURCE_PATH}/Entities/HealthbarEntity.h
${GAME_SOURCE_PATH}/Entities/HealthbarEntity.cpp
${GAME_SOURCE_PATH}/Entities/PlayerEntity.h
${GAME_SOURCE_PATH}/Entities/PlayerEntity.cpp
${GAME_SOURCE_PATH}/Entities/EnemyEntity.h
${GAME_SOURCE_PATH}/Entities/EnemyEntity.cpp
${GAME_SOURCE_PATH}/Entities/WaveSystemEntity.h
${GAME_SOURCE_PATH}/Entities/WaveSystemEntity.cpp
${GAME_SOURCE_PATH}/Entities/ProjectileEntity.h
${GAME_SOURCE_PATH}/Entities/ProjectileEntity.cpp
${GAME_SOURCE_PATH}/Entities/PickUpEntity.h
${GAME_SOURCE_PATH}/Entities/PickUpEntity.cpp
${GAME_SOURCE_PATH}/Entities/GameManager.h
${GAME_SOURCE_PATH}/Entities/GameManager.cpp
)

add_library(${ENGINE_PROJECT_NAME} ${ENGINE_SOURCE_FILES})
add_executable(${GAME_PROJECT_NAME} ${GAME_SOURCE_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_PROJECT_NAME})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/${GAME_SOURCE_PATH} FILES ${GAME_SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/${ENGINE_SOURCE_PATH} FILES ${ENGINE_SOURCE_FILES})

target_link_libraries(${ENGINE_PROJECT_NAME} sfml-graphics sfml-audio sfml-network nlohmann_json::nlohmann_json tinyxml2)
target_link_libraries(${GAME_PROJECT_NAME} ${ENGINE_PROJECT_NAME} )