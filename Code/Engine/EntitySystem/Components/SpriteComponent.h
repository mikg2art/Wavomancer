#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"

class SpriteComponent : public EntityComponent
{
public:

	virtual void Shutdown() override;
	virtual void OnPositionChanged() override;

	sf::Texture* SetSpriteByPath(const std::string& path, bool centerSprite);
	sf::Sprite& GetSprite() { return m_sprite; }

	void SetVisibility(bool isVisible);

private:

	void UnregisterSprite();

	int m_entryId = 0;
	sf::Sprite m_sprite;
	bool m_isVisible = false;
};