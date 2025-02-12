#include "WaveSystemEntity.h"

#include "Engine/EntitySystem/EntityFactory.h"
#include "Engine/EntitySystem/Components/SpriteComponent.h"
#include "Engine/EntitySystem/Components/RigidbodyComponent.h"
#include "Engine/EntitySystem/Components/CircleCollisionComponent.h"
#include "Engine/EntitySystem/Components/ViewComponent.h"
#include "Engine/Engine.h"

#include "Game/Entities/GameManager.h"
#include "Game/GameStates.h"

#include "Engine/SFMLMath/SFMLMath.hpp"

using namespace sf;

void MyStream::load(const sf::SoundBuffer& buffer)
{
	// extract the audio samples from the sound buffer to our own container
	m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

	// reset the current playing position 
	m_currentSample = 0;

	// initialize the base class
	initialize(buffer.getChannelCount(), buffer.getSampleRate());
}

unsigned char MyStream::getData()
{
	//return m_samples[m_currentSample];
	return (m_samples[m_currentSample + 1] * 1.0 / USHRT_MAX) * 255;

	/*
	using stream_sound we need to get current sample and show it's data as text.
	text changing means we are on a right way.
	*/
}

bool MyStream::onGetData(Chunk& data)
{
	// number of samples to stream every time the function is called;
	// in a more robust implementation, it should be a fixed
	// amount of time rather than an arbitrary number of samples
	const int samplesToStream = 50000;

	// set the pointer to the next audio samples to be played
	data.samples = &m_samples[m_currentSample];

	// have we reached the end of the sound?
	if (m_currentSample + samplesToStream <= m_samples.size())
	{
		// end not reached: stream the samples and continue
		data.sampleCount = samplesToStream;
		m_currentSample += samplesToStream;
		return true;
	}
	else
	{
		// end of stream reached: stream the remaining samples and stop playback
		data.sampleCount = m_samples.size() - m_currentSample;
		m_currentSample = m_samples.size();
		return false;
	}
}

void MyStream::onSeek(sf::Time timeOffset)
{
	//calculate the corresponding sample index according to the sample rate and channel count
	m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
}


void WaveSystemEntity::Init()
{
	m_soundBuffer.loadFromFile("../Assets/Music/dark-techno-instrumental-intense-electronic-beats-296096.wav");
	//m_soundBuffer.loadFromFile("../Assets/Music/5m_silence.wav");

	m_samples_rate = m_soundBuffer.getSampleRate();
	m_sound_duration = m_soundBuffer.getDuration();

	m_sound.setVolume(15);
	m_sound.setBuffer(m_soundBuffer);
	m_sound.play();

	const unsigned int numberOfChannels = m_soundBuffer.getChannelCount();
	m_numberOfChannels = m_soundBuffer.getChannelCount();
	m_sampleData = m_soundBuffer.getSamples();

	size_t sampleCount = m_soundBuffer.getSampleCount();
	//unsigned short int min = USHRT_MAX;
	//unsigned short int max = 0;
	short int current = 0;

	for (size_t i = 0; i < sampleCount; i++)
	{
		current = abs(m_sampleData[i]);
		if (current > m_max) {
			m_max = current;
		}
		if (current < m_min) {
			m_min = current;
		}
	}

	m_delta = m_max - m_min;

	//WaveSystemSprite
	m_pWaveSystemSpriteComponent = AddComponent<SpriteComponent>();
	m_pWaveSystemSpriteComponent->SetSpriteByPath("../Assets/Kenney/Shmup/Tiles/mp_tile_0087.png", true);
	m_pWaveSystemSpriteComponent->GetSprite().setScale(3.f, 3.f);

	m_soundStop = false;

	//CollisionComponent
	//m_pCollisionComponent = AddComponent<CircleCollisionComponent>();
	//m_pCollisionComponent->SetProperties(40.f, false, false, nullptr);
}


void WaveSystemEntity::Update(float deltaSeconds)
{
	if (!m_soundStop) {
		m_currentPlayingTime = m_sound.getPlayingOffset();
		m_currentSamplePosition = m_currentPlayingTime.asSeconds() * m_sampleFrequency;
		m_currentSampleDataIndex = m_currentSamplePosition * m_numberOfChannels;
		m_leftChannel = m_sampleData[m_currentSampleDataIndex];
		m_rightChannel = m_sampleData[m_currentSampleDataIndex + 1];

		m_musik_data_left = ((fabs(m_leftChannel * 1.0) - m_min) / m_delta) * 100;
		m_musik_data_right = ((fabs(m_rightChannel * 1.0) - m_min) / m_delta) * 100;
	}
}


void WaveSystemEntity::Shutdown()
{
	if (m_inputCallbackId != 0)
	{
		Engine::GetInstance()->GetInputManager().Unregister(m_inputCallbackId);
		m_inputCallbackId = 0;
	}
}

void WaveSystemEntity::SetColor(sf::Color color)
{
	m_factionColor = color;

	m_pWaveSystemSpriteComponent->GetSprite().setColor(m_factionColor);
}

void WaveSystemEntity::SetTargetId(int targetEntityId, sf::Color color)
{
	m_targetEntityId = targetEntityId;
	//m_pWaveSystemIndicatorSpriteComponent->GetSprite().setColor(color);
}

void WaveSystemEntity::WaveSoundStart()
{
	//if (m_soundStop == true) {
		m_soundStop = false;
		m_sound.play();
		std::cout << "play" << std::endl;
	//}
}

void WaveSystemEntity::WaveSoundStop()
{
	//if (m_soundStop == false) {
		m_soundStop = true;
		m_sound.pause();
	//}
}

void WaveSystemEntity::WaveSoundEnd()
{
	//if (m_soundStop == false) {
		m_soundStop = true;
		m_sound.stop();
	//}
}

unsigned char WaveSystemEntity::GetLeftChannelMusicData()
{
	return m_musik_data_left;
}

unsigned char WaveSystemEntity::GetRightChannelMusicData()
{
	return m_musik_data_right;
}