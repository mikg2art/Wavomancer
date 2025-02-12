#pragma once

#include <vector>
#include <SFML/Audio.hpp>

class GameState
{
public:
	virtual ~GameState() = default;
	
	virtual void OnEnter(GameState* pPrevState) {};
	virtual void Update(float deltaSeconds) {}
	virtual void OnExit(GameState* pNextState);

	int m_WaveSystemId = 0;

protected:

	std::vector<int> m_ownedEntityIds;
};

class TitleScreenGameState : public GameState
{
	virtual void OnEnter(GameState* pPrevState) override;
	virtual void Update(float deltaSeconds) override;
	virtual void OnExit(GameState* pNextState) override;

private:

	class TextRenderComponent* m_pPromptComponent = nullptr;
	int m_inputCallbackId = 0;
	sf::Music m_bgMusic;

	//int m_WaveSystemId = 0;
	//class WaveSystemEntity* pWaveSystemTS;
};

class IngameGameState : public GameState
{
	virtual void OnEnter(GameState* pPrevState) override;
	virtual void Update(float deltaSeconds) override;

public:
	//int m_WaveSystemId = 0;
	int m_PlayerId = 0;
	int m_EnemyId = 0;

	std::vector<int> m_EnemyIds;

	std::vector<int> m_boundIds;

	int m_score = 0;

private:
	class TextRenderComponent* m_pScoreTextComponent = nullptr;
	class WaveSystemEntity* m_WaveSystemI;
	class PlayerEntity* m_PlayerI;

	float m_secondsUntilBuffSpawn = 0.f;
	float m_secondsBetweenBuffSpawn = 4.f;

	float m_secondsUntilEnemySpawn = 0.f;
	float m_secondsBetweenEnemySpawn = 2.f;

	float m_secondsUntilEnemySpawnFaster = 0.f;
	float m_secondsBetweenEnemySpawnFaster = 5.f;

	float m_secondsUntilFireRateChange = 0.0f;
	float m_secondsBetweenFireRateChange = 0.4f;

	void CreatePlayfieldBounds();
	void SpawnRandomBuff();
	void SpawnEnemy();
	void GetScore();
	void PlusScore(int num);

};

class GameOverGameState : public GameState
{
protected:

	virtual void OnEnter(GameState* pPrevState) override;
	virtual void OnExit(GameState* pNextState) override;

public:

	int loserEntityId = 0;

private:

	int m_inputCallbackId = 0;

};