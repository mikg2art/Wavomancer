#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

using TCallbackSignature = std::function<void()>;

enum class EInputEvent
{
	Pressed,
	Released
};

struct KeyboardInputEntry
{
	int id = 0;
	sf::Keyboard::Key keyToListenTo = sf::Keyboard::A;
	bool wasPressedDuringLastFrame = false;
	EInputEvent eventToListenTo = EInputEvent::Pressed;
	TCallbackSignature callbackFunction;
};

struct MouseInputEntry
{
	int id = 0;
	sf::Mouse::Button buttonToListenTo = sf::Mouse::Left;
	bool wasPressedDuringLastFrame = false;
	EInputEvent eventToListenTo = EInputEvent::Pressed;
	TCallbackSignature callbackFunction;
};