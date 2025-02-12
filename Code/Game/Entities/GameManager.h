#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"
#include "Game/GameStates.h"

#include <SFML/Audio.hpp>

class GameManager : public Entity
{
public:

	static GameManager* Get();
	
	virtual void Init() override;
	virtual void Update(float deltaSeconds) override;
	
	void SetGameState(std::shared_ptr<GameState> pNewState);

private:

	std::shared_ptr<GameState> m_pCurrentGameState;
	static GameManager* s_pInstance;

	sf::Music m_bgMusic;

};