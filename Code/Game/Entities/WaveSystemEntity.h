#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"
#include "Game/DamageDefinitions.h"
#include "Engine/Input/InputManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <Game/BuffDefinitions.h>

class SpriteComponent;
class RigidbodyComponent;
class CircleCollisionComponent;
class ViewComponent;


class MyStream : public sf::SoundStream
{
public:

	void load(const sf::SoundBuffer& buffer);

	unsigned char getData();

private:

	bool onGetData(Chunk& data);

	void onSeek(sf::Time timeOffset);

	std::vector<sf::Int16> m_samples;
	std::size_t m_currentSample;
};

class WaveSystemEntity : public Entity
{
	using TMovementKeys = std::array<sf::Keyboard::Key, 4>;

public:

	// Entity
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void Update(float deltaSeconds) override;
	// ~Entity

	// IDamageable
	//virtual void ProcessDamage(float damageAmount) override;
	//virtual float GetHealth() const override { return m_currentHealth; }
	//virtual float GetMaxHealth() const override { return m_maxHealth; }
	// ~IDamageable

	//void SetInputKeys(sf::Mouse::Button fireButton, const TMovementKeys& movementKeys);
	void SetColor(sf::Color color);
	void SetTargetId(int targetEntityId, sf::Color color);

	//void AddBuff(std::shared_ptr<Buff> pBuff);

	void WaveSoundStart();
	void WaveSoundStop();
	void WaveSoundEnd();
	unsigned char GetLeftChannelMusicData();
	unsigned char GetRightChannelMusicData();

protected:

	//void OnInputPressed();

	sf::SoundBuffer m_soundBuffer;
	unsigned int m_samples_rate = 0;
	sf::Time m_sound_duration;
	sf::Sound m_sound;

	const unsigned int m_sampleFrequency{ 44100u };
	unsigned int m_numberOfChannels = 0;
	const sf::Int16* m_sampleData = 0;
	sf::Time m_currentPlayingTime;
	sf::Uint32 m_currentSamplePosition = 0;
	sf::Uint32 m_currentSampleDataIndex = 0;

	sf::Int16 m_leftChannel = 0;
	sf::Int16 m_rightChannel = 0;

	unsigned char m_musik_data_left = 0;
	unsigned char m_musik_data_right = 0;

	unsigned short int m_min = USHRT_MAX;
	unsigned short int m_max = 0;
	double m_delta = 0;

	bool m_soundStop = false;

	int m_inputCallbackId = 0;
	int m_targetEntityId = 0;

	SpriteComponent* m_pWaveSystemSpriteComponent = nullptr;
	//SpriteComponent* m_pWaveSystemIndicatorSpriteComponent = nullptr;

	bool m_handleInput = true;

	sf::Color m_factionColor;
};