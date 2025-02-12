#include "GameStates.h"

#include "Engine/Engine.h"
#include "Engine/EntitySystem/EntitySystem.h"
#include "Engine/EntitySystem/Components/BoxCollisionComponent.h"
#include "Engine/EntitySystem/Components/SpriteComponent.h"
#include "Engine/EntitySystem/Components/TextRenderComponent.h"
#include "Engine/EntitySystem/Components/ViewComponent.h"
#include "Engine/SFMLMath/SFMLMath.hpp"

#include "Game/Entities/PlayerEntity.h"
#include "Game/Entities/EnemyEntity.h"
#include "Game/Entities/WaveSystemEntity.h"
#include "Game/Entities/GameManager.h"
#include "Game/Entities/PickupEntity.h"
#include "Game/Entities/ProjectileEntity.h"


//////////////////////////////////////////////////////////////////////////
void GameState::OnExit(GameState* pNextState)
{
	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
	for (int id : m_ownedEntityIds)
	{
		if (Entity* pEntity = entitySystem.FindEntityById(id))
		{
			pEntity->Destroy();
		}
	}

	m_ownedEntityIds.clear();
}

//////////////////////////////////////////////////////////////////////////
void TitleScreenGameState::OnEnter(GameState* pPrevState)
{
	{
		sf::RenderWindow& renderWindow = Engine::GetInstance()->GetRenderWindow();

		Entity* pPlayfield = Engine::GetInstance()->GetEntitySystem().SpawnEntity<Entity>();
		SpriteComponent* pSpriteComp = pPlayfield->AddComponent<SpriteComponent>();
		pSpriteComp->SetSpriteByPath("../Assets/Custom/ground2.png", true);
		pSpriteComp->GetSprite().setScale(4.15, 4.1);
		//pSpriteComp->GetSprite().setColor(sf::Color(168, 188, 240, 255));

		pSpriteComp = pPlayfield->AddComponent<SpriteComponent>();
		pSpriteComp->SetSpriteByPath("../Assets/Custom/borders.png", true);
		pSpriteComp->GetSprite().setScale(4.15, 4.1);
		pSpriteComp->GetSprite().setColor(sf::Color(168, 188, 240, 255));

		/*pSpriteComp = pPlayfield->AddComponent<SpriteComponent>();
		pSpriteComp->SetSpriteByPath("../Assets/Custom/test.png", true);
		pSpriteComp->GetSprite().setScale(4.15, 4.1);
		pSpriteComp->GetSprite().setColor(sf::Color(25, 0, 170, 255));*/

		if (ViewComponent* pViewComponent = pPlayfield->AddComponent<ViewComponent>())
		{
			sf::View view;
			view.setSize(sf::Vector2f(static_cast<float>(renderWindow.getSize().x), static_cast<float>(renderWindow.getSize().y)));
			view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
			view.setCenter(pPlayfield->GetPosition());
			pViewComponent->SetView(view);
		}
	}

	{
		//WaveSyst
		EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
		WaveSystemEntity* pWaveSystemTS = entitySystem.SpawnEntity<WaveSystemEntity>();
		pWaveSystemTS->SetColor(sf::Color(168, 188, 240));
		m_WaveSystemId = pWaveSystemTS->GetId();
		pWaveSystemTS->WaveSoundStop();
	}

	{
		//Title
		Entity* pTitle = Engine::GetInstance()->GetEntitySystem().SpawnEntity<Entity>();
		SpriteComponent* pSpriteComp = pTitle->AddComponent<SpriteComponent>();
		pSpriteComp->SetSpriteByPath("../Assets/Custom/logo.png", true);
		pSpriteComp->GetSprite().setScale(1.7, 1.7);
		pSpriteComp->GetSprite().setPosition(sf::Vector2f(0,50));

		pTitle->SetPosition(pTitle->GetPosition() - sf::Vector2f(0, 100.f));

		m_ownedEntityIds.push_back(pTitle->GetId());
	}

	{
		//InputPrompt
		Entity* pInputPrompt = Engine::GetInstance()->GetEntitySystem().SpawnEntity<Entity>();
		m_pPromptComponent = pInputPrompt->AddComponent<TextRenderComponent>();
		m_pPromptComponent->SetFontByPath("../Assets/Fonts/Alagard.ttf");
		m_pPromptComponent->GetText().setString("Press Enter to continue");
		m_pPromptComponent->CenterText();

		pInputPrompt->SetPosition(pInputPrompt->GetPosition() + sf::Vector2f(0, 200.f));

		m_ownedEntityIds.push_back(pInputPrompt->GetId());
	}

	{
		//Input
		m_inputCallbackId = Engine::GetInstance()->GetInputManager().Register(
			sf::Keyboard::Enter,
			EInputEvent::Released,
			[&]() {
				GameManager::Get()->SetGameState(std::make_shared<IngameGameState>());
			});
	}

	{
		//TitleMusic
		m_bgMusic.openFromFile("../Assets/Music/modern-electronic-utopia-288723-cut.wav");
		m_bgMusic.setLoop(true);
		m_bgMusic.setVolume(15);
		m_bgMusic.play();
	}
}

void TitleScreenGameState::Update(float deltaSeconds)
{
	sf::Color color(255, 255, 255, 100 + static_cast<int>(((std::sin(Engine::GetInstance()->GetElapsedTime().asSeconds() * 3.f) + 1.f) * 0.5f) * 155));
	m_pPromptComponent->GetText().setFillColor(color);
}

void TitleScreenGameState::OnExit(GameState* pNextState)
{
	m_bgMusic.stop();

	GameState::OnExit(pNextState);

	Engine::GetInstance()->GetInputManager().Unregister(m_inputCallbackId);
}

//////////////////////////////////////////////////////////////////////////
void IngameGameState::OnEnter(GameState* pPrevState)
{
	{
		CreatePlayfieldBounds();
	}
	//Here player spawns.
	{
		const sf::Color FillColor = sf::Color(168, 188, 240);

		EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
		if (m_WaveSystemI = static_cast<WaveSystemEntity*>(entitySystem.FindEntityById(pPrevState->m_WaveSystemId))) {
			m_WaveSystemI->WaveSoundStart();
			m_WaveSystemId = m_WaveSystemI->GetId();
		}

		// Player entities
		PlayerEntity* m_PlayerI = Engine::GetInstance()->GetEntitySystem().SpawnEntity<PlayerEntity>();
		m_PlayerI->SetInputKeys(sf::Mouse::Left, { sf::Keyboard::W, sf::Keyboard::A, sf::Keyboard::S, sf::Keyboard::D });
		m_PlayerI->SetColor(FillColor);
		m_PlayerI->SetCanMove(true);

		m_PlayerId = m_PlayerI->GetId();
	}

	{
		//ScoreBack
		Entity* pScore = Engine::GetInstance()->GetEntitySystem().SpawnEntity<Entity>();
		SpriteComponent* pSpriteComp = pScore->AddComponent<SpriteComponent>();
		pSpriteComp->SetSpriteByPath("../Assets/Custom/score.png", true);
		pSpriteComp->GetSprite().setScale(4, 4);

		sf::Vector2u sizeInt = Engine::GetInstance()->GetRenderWindow().getSize();
		sf::Vector2f size(static_cast<float>(sizeInt.x), static_cast<float>(sizeInt.y));
		size.x *= 0.5f;
		size.y *= 0.5f;

		pSpriteComp->GetSprite().setPosition(sf::Vector2f(size.x - 125, -size.y + 75));

		m_ownedEntityIds.push_back(pScore->GetId());
	

		//ScoreText
		Entity* ScoreText = Engine::GetInstance()->GetEntitySystem().SpawnEntity<Entity>();
		m_pScoreTextComponent = ScoreText->AddComponent<TextRenderComponent>();
		m_pScoreTextComponent->SetFontByPath("../Assets/Fonts/Alagard.ttf");
		m_pScoreTextComponent->GetText().setCharacterSize(30);
		m_pScoreTextComponent->GetText().setString("  0");
		m_pScoreTextComponent->CenterText();

		ScoreText->SetPosition(ScoreText->GetPosition() + sf::Vector2f(size.x - 129, -size.y + 75));

		m_ownedEntityIds.push_back(ScoreText->GetId());
	}

	m_secondsUntilBuffSpawn = m_secondsBetweenBuffSpawn;
	std::srand(static_cast<int>(std::time(nullptr)));
}

void IngameGameState::Update(float deltaSeconds)
{
	float musicData = (float)(m_WaveSystemI->GetLeftChannelMusicData()) / 100;

	m_secondsUntilBuffSpawn -= deltaSeconds;
	if (m_secondsUntilBuffSpawn <= 0.f)
	{
		m_secondsUntilBuffSpawn = m_secondsBetweenBuffSpawn - musicData;

		if (Engine::GetInstance()->GetEntitySystem().CountAllEntitiesOfType<PickUpEntity>() < 5)
			SpawnRandomBuff();
	}

	m_secondsUntilEnemySpawn -= deltaSeconds;
	if (m_secondsUntilEnemySpawn <= 0.f)
	{
		m_secondsUntilEnemySpawn = m_secondsBetweenEnemySpawn - musicData;

		if (Engine::GetInstance()->GetEntitySystem().CountAllEntitiesOfType<EnemyEntity>() < 15)
			SpawnEnemy();
	}

	m_secondsUntilEnemySpawnFaster -= deltaSeconds;
	if (m_secondsUntilEnemySpawnFaster <= 0.f)
	{
		
		m_secondsUntilEnemySpawnFaster = m_secondsBetweenEnemySpawnFaster + musicData;
		
		if (m_secondsBetweenEnemySpawn > 0.3f) {
			m_secondsBetweenEnemySpawn -= 0.2f;
		}
	}

	m_secondsUntilFireRateChange -= deltaSeconds;
	if (m_secondsUntilFireRateChange <= 0.f)
	{
		m_secondsUntilFireRateChange = m_secondsBetweenFireRateChange;
		EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
		if (m_PlayerI = static_cast<PlayerEntity*>(entitySystem.FindEntityById(m_PlayerId))) {
			m_PlayerI->SetFireRate(musicData);
		}
	}

	GetScore();
}

void IngameGameState::CreatePlayfieldBounds()
{
	constexpr float colliderThickness = 100.f;

	sf::Vector2u size = Engine::GetInstance()->GetRenderWindow().getSize();

	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();

	{
		//Upper bound
		Entity* pBound = entitySystem.SpawnEntity<Entity>();
		pBound->SetPosition(sf::Vector2f(0.f, (size.y + colliderThickness) * -0.5f));

		BoxCollisionComponent* pBoxCollider = pBound->AddComponent<BoxCollisionComponent>();
		pBoxCollider->SetProperties(static_cast<float>(size.x), colliderThickness, true, false, nullptr);

		m_boundIds.push_back(pBound->GetId());
	}

	{
		//Lower bound
		Entity* pBound = entitySystem.SpawnEntity<Entity>();
		pBound->SetPosition(sf::Vector2f(0.f, (size.y + colliderThickness) * 0.5f));

		BoxCollisionComponent* pBoxCollider = pBound->AddComponent<BoxCollisionComponent>();
		pBoxCollider->SetProperties(static_cast<float>(size.x), colliderThickness, true, false, nullptr);

		m_boundIds.push_back(pBound->GetId());
	}

	{
		//Right bound
		Entity* pBound = entitySystem.SpawnEntity<Entity>();
		pBound->SetPosition(sf::Vector2f((size.x + colliderThickness) * 0.5f, 0.f));

		BoxCollisionComponent* pBoxCollider = pBound->AddComponent<BoxCollisionComponent>();
		pBoxCollider->SetProperties(colliderThickness, static_cast<float>(size.y), true, false, nullptr);

		m_boundIds.push_back(pBound->GetId());
	}

	{
		//Left bound
		Entity* pBound = entitySystem.SpawnEntity<Entity>();
		pBound->SetPosition(sf::Vector2f((size.x + colliderThickness) * -0.5f, 0.f));

		BoxCollisionComponent* pBoxCollider = pBound->AddComponent<BoxCollisionComponent>();
		pBoxCollider->SetProperties(colliderThickness, static_cast<float>(size.y), true, false, nullptr);

		m_boundIds.push_back(pBound->GetId());
	}
}

void IngameGameState::SpawnRandomBuff()
{
	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();

	sf::Vector2u sizeInt = Engine::GetInstance()->GetRenderWindow().getSize();
	sf::Vector2f size(static_cast<float>(sizeInt.x), static_cast<float>(sizeInt.y));
	size.x *= 0.9f;
	size.y *= 0.9f;

	const float randX = sf::GetRandomInRange(0.f, size.x) - size.x * 0.5f;
	const float randY = sf::GetRandomInRange(0.f, size.y) - size.y * 0.5f;

	PickUpEntity* pSpawnedPickUp = nullptr;

	const int type = std::rand() % 3;
	switch (type)
	{
	case 0:
		pSpawnedPickUp = entitySystem.SpawnEntity<MoveSpeedPickUp>();
		break;
	case 1:
		pSpawnedPickUp = entitySystem.SpawnEntity<MoveSpeedPickUp>();
		break;
	case 2:
		pSpawnedPickUp = entitySystem.SpawnEntity<TripleShotPickUp>();
		break;
	}

	m_ownedEntityIds.push_back(pSpawnedPickUp->GetId());
	pSpawnedPickUp->SetPosition(sf::Vector2f(randX, randY));
}

void IngameGameState::SpawnEnemy()
{
	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();

	sf::Vector2u sizeInt = Engine::GetInstance()->GetRenderWindow().getSize();
	sf::Vector2f size(static_cast<float>(sizeInt.x), static_cast<float>(sizeInt.y));
	size.x *= 0.5f;
	size.y *= 0.5f;

	float randSide;
	if (std::rand() % 2 == 0)
		randSide = size.x - 75;
	else 
		randSide = -size.x + 75;

	const float randX = randSide;
	const float randY = sf::GetRandomInRange(-size.y, size.y);

	EnemyEntity* pSpawnedEnemy = entitySystem.SpawnEntity<EnemyEntity>();
	const int type = std::rand() % 4;
	if (type == 0) {
		pSpawnedEnemy->SetEnemyType(0);
	}
	else if (type == 1) {
		pSpawnedEnemy->SetEnemyType(3);
	}
	else if (type == 2) {
		pSpawnedEnemy->SetEnemyType(1);
	}
	else if (type == 3) {
		pSpawnedEnemy->SetEnemyType(2);
	}
	pSpawnedEnemy->SetPosition(sf::Vector2f(randX, randY));
	pSpawnedEnemy->SetStartPosition();
	pSpawnedEnemy->SetColor(sf::Color(168, 188, 240));

	m_ownedEntityIds.push_back(pSpawnedEnemy->GetId());

	m_EnemyIds.push_back(pSpawnedEnemy->GetId());
}

void IngameGameState::GetScore()
{
	if (m_EnemyIds.size() > 0) {
		EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();
		EnemyEntity* pEnemy;

		for (int i = 0; i < m_EnemyIds.size(); i++)
		{
			if (pEnemy = static_cast<EnemyEntity*>(entitySystem.FindEntityById(m_EnemyIds[i]))) {
				
			}
			else 
			{
				m_EnemyIds.erase(m_EnemyIds.begin() + i);
				PlusScore(100);
			}
		}
	}
}

void IngameGameState::PlusScore(int num) 
{
	m_score += num;
	m_pScoreTextComponent->GetText().setString(std::to_string(m_score));
}

//////////////////////////////////////////////////////////////////////////
//GameOver
void GameOverGameState::OnEnter(GameState* pPrevState)
{
	EntitySystem& entitySystem = Engine::GetInstance()->GetEntitySystem();

	std::string displayText;
	sf::Color displayColor;

	if (IngameGameState* pIngameState = static_cast<IngameGameState*>(pPrevState))
	{
		if (WaveSystemEntity* pWaveSystemGO = static_cast<WaveSystemEntity*>(entitySystem.FindEntityById(pPrevState->m_WaveSystemId))) {
			pWaveSystemGO->WaveSoundEnd();
			m_WaveSystemId = pWaveSystemGO->GetId();
		}

		m_ownedEntityIds.push_back(pIngameState->m_PlayerId);
		m_ownedEntityIds.insert(m_ownedEntityIds.begin(), pIngameState->m_boundIds.begin(), pIngameState->m_boundIds.end());

		PlayerEntity* pPlayerEntity = static_cast<PlayerEntity*>(entitySystem.FindEntityById(pIngameState->m_PlayerId));

		//Make player invulnerable
		pPlayerEntity->SetCanMove(false);
		pPlayerEntity->AddBuff(std::make_shared<ArmorBuff>(0.f));

		displayText = std::to_string(pIngameState->m_score);
	}

	//Input
	m_inputCallbackId = Engine::GetInstance()->GetInputManager().Register(
		sf::Keyboard::R,
		EInputEvent::Released,
		[]() {
			GameManager::Get()->SetGameState(std::make_shared<IngameGameState>());
		});

	Entity* pEntity = entitySystem.SpawnEntity<Entity>();
	TextRenderComponent* pWinnerText = pEntity->AddComponent<TextRenderComponent>();
	pWinnerText->SetFontByPath("../Assets/Fonts/Alagard.ttf");
	pWinnerText->GetText().setString(displayText);
	pWinnerText->GetText().setCharacterSize(160);
	pWinnerText->GetText().setPosition(sf::Vector2f(0, -50.f));
	pWinnerText->CenterText();

	TextRenderComponent* pTextPrompt = pEntity->AddComponent<TextRenderComponent>();
	pTextPrompt->SetFontByPath("../Assets/Fonts/Alagard.ttf");
	pTextPrompt->GetText().setString("Press R for a rematch");
	pTextPrompt->GetText().setPosition(sf::Vector2f(0, 50.f));
	pTextPrompt->CenterText();

	TextRenderComponent* pTextScore = pEntity->AddComponent<TextRenderComponent>();
	pTextScore->SetFontByPath("../Assets/Fonts/Alagard.ttf");
	pTextScore->GetText().setString("Score:");
	pTextScore->GetText().setPosition(sf::Vector2f(0, -150.f));
	pTextScore->CenterText();

	m_ownedEntityIds.push_back(pEntity->GetId());
}

void GameOverGameState::OnExit(GameState* pNextState)
{
	GameState::OnExit(pNextState);

	Engine::GetInstance()->GetInputManager().Unregister(m_inputCallbackId);
	Engine::GetInstance()->GetEntitySystem().DestroyAllEntitiesOfType<ProjectileEntity>();
	Engine::GetInstance()->GetEntitySystem().DestroyAllEntitiesOfType<PickUpEntity>();
	Engine::GetInstance()->GetEntitySystem().DestroyAllEntitiesOfType<EnemyEntity>();
}
