#include "InputManager.h"

#include "Engine/Engine.h"

int InputManager::Register(sf::Mouse::Button buttonOfInterested, EInputEvent eventToListenTo, TCallbackSignature callback)
{
	m_idCounter++;

	MouseInputEntry entry;
	entry.id = m_idCounter;
	entry.buttonToListenTo = buttonOfInterested;
	entry.eventToListenTo = eventToListenTo;
	entry.callbackFunction = callback;

	m_mouseEntries.push_back(entry);

	return entry.id;
}

int InputManager::Register(sf::Keyboard::Key keyOfInterested, EInputEvent eventToListenTo, TCallbackSignature callback)
{
	m_idCounter++;

	KeyboardInputEntry entry;
	entry.id = m_idCounter;
	entry.keyToListenTo = keyOfInterested;
	entry.eventToListenTo = eventToListenTo;
	entry.callbackFunction = callback;

	m_keyboardEntries.push_back(entry);

	return entry.id;
}

void InputManager::Unregister(int callbackId)
{
	for (int i = 0; i < m_keyboardEntries.size(); i++)
	{
		if (m_keyboardEntries[i].id == callbackId)
		{
			m_keyboardEntries.erase(m_keyboardEntries.begin() + i);
			return;
		}
	}

	for (int i = 0; i < m_mouseEntries.size(); i++)
	{
		if (m_mouseEntries[i].id == callbackId)
		{
			m_mouseEntries.erase(m_mouseEntries.begin() + i);
			return;
		}
	}
}

void InputManager::Update()
{
	if (!Engine::GetInstance()->GetRenderWindow().hasFocus())
	{
		return;
	}

	std::vector<KeyboardInputEntry> keyboardEntriesCopy = m_keyboardEntries;
	for (KeyboardInputEntry& entry : m_keyboardEntries)
	{
		const bool isPressedNow = sf::Keyboard::isKeyPressed(entry.keyToListenTo);

		if (entry.wasPressedDuringLastFrame && !isPressedNow)
		{
			//released key
			if (entry.eventToListenTo == EInputEvent::Released)
			{
				entry.callbackFunction();
			}
		}
		else if (!entry.wasPressedDuringLastFrame && isPressedNow)
		{
			// pressed key
			if (entry.eventToListenTo == EInputEvent::Pressed)
			{
				entry.callbackFunction();
			}
		}

		if (entry.wasPressedDuringLastFrame != isPressedNow)
		{
			// we actually need to save this to the original vector
			if (KeyboardInputEntry* pFoundEntry = FindKeyboardInputEntryById(entry.id))
			{
				pFoundEntry->wasPressedDuringLastFrame = isPressedNow;
			}
		}
	}

	std::vector<MouseInputEntry> mouseEntriesCopy = m_mouseEntries;
	for (MouseInputEntry& entry : mouseEntriesCopy)
	{
		const bool isPressedNow = sf::Mouse::isButtonPressed(entry.buttonToListenTo);

		if (entry.wasPressedDuringLastFrame && !isPressedNow)
		{
			//released key
			if (entry.eventToListenTo == EInputEvent::Released)
			{
				entry.callbackFunction();
			}
		}
		else if (!entry.wasPressedDuringLastFrame && isPressedNow)
		{
			// pressed key
			if (entry.eventToListenTo == EInputEvent::Pressed)
			{
				entry.callbackFunction();
			}
		}

		if (entry.wasPressedDuringLastFrame != isPressedNow)
		{
			// we actually need to save this to the original vector
			if (MouseInputEntry* pFoundEntry = FindMouseInputEntryById(entry.id))
			{
				pFoundEntry->wasPressedDuringLastFrame = isPressedNow;
			}
		}
	}
}

bool InputManager::IsButtonPressed(sf::Mouse::Button button) const
{
	return sf::Mouse::isButtonPressed(button);
}

bool InputManager::IsKeyPressed(sf::Keyboard::Key key) const
{
	return sf::Keyboard::isKeyPressed(key);
}

MouseInputEntry* InputManager::FindMouseInputEntryById(int id)
{
	for (int i = 0; i < m_mouseEntries.size(); i++)
	{
		if (m_mouseEntries[i].id == id)
			return &m_mouseEntries[i];
	}

	return nullptr;
}

KeyboardInputEntry* InputManager::FindKeyboardInputEntryById(int id)
{
	for (int i = 0; i < m_keyboardEntries.size(); i++)
	{
		if (m_keyboardEntries[i].id == id)
			return &m_keyboardEntries[i];
	}

	return nullptr;
}
