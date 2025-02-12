#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "Engine/Input/InputSystemDefinitions.h"

class InputManager
{
public:

	int Register(sf::Mouse::Button buttonOfInterested, EInputEvent eventToListenTo, TCallbackSignature callback);
	int Register(sf::Keyboard::Key keyOfInterested, EInputEvent eventToListenTo, TCallbackSignature callback);
	void Unregister(int callbackId);
	
	void Update();

	bool IsButtonPressed(sf::Mouse::Button button) const;
	bool IsKeyPressed(sf::Keyboard::Key key) const;

protected:

	MouseInputEntry* FindMouseInputEntryById(int id);
	KeyboardInputEntry* FindKeyboardInputEntryById(int id);

	int m_idCounter = 0;

	std::vector<KeyboardInputEntry> m_keyboardEntries;
	std::vector<MouseInputEntry> m_mouseEntries;

};