#include "GameManager.h"
#include "Engine/Engine.h"
#include "Engine/EntitySystem/EntitySystem.h"

//#include "Game/Entities/TankEntity.h"
#include "Game/Entities/PickupEntity.h"
#include "Engine/EntitySystem/Components/ViewComponent.h"
#include "Engine/EntitySystem/Components/SpriteComponent.h"
#include "Engine/EntitySystem/Components/CircleCollisionComponent.h"
#include "Engine/EntitySystem/Components/BoxCollisionComponent.h"

#include "Game/GameStates.h"

#include <array>

GameManager* GameManager::s_pInstance = nullptr;

GameManager* GameManager::Get()
{
	return s_pInstance;
}

//“ут спавнитс€ музыка и можно тут же wav шутку добавить
void GameManager::Init()
{
	s_pInstance = this;

	SetGameState(std::make_shared<TitleScreenGameState>());
	//SetGameState(std::make_shared<IngameGameState>());

	/*m_bgMusic.openFromFile("../Assets/Music/track_2.wav");
	m_bgMusic.setLoop(true);
	m_bgMusic.setVolume(15);
	m_bgMusic.play();*/
}

void GameManager::Update(float deltaSeconds)
{
	if (m_pCurrentGameState)
	{
		m_pCurrentGameState->Update(deltaSeconds);
	}
}

void GameManager::SetGameState(std::shared_ptr<GameState> pNewState)
{
	if (m_pCurrentGameState)
	{
		m_pCurrentGameState->OnExit(pNewState.get());
	}

	std::shared_ptr<GameState> pOldState = m_pCurrentGameState;
	m_pCurrentGameState = pNewState;

	if (m_pCurrentGameState)
	{
		m_pCurrentGameState->OnEnter(pOldState.get());
	}
}

