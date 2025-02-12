#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"

class TextRenderComponent : public EntityComponent
{
public:

	virtual void Shutdown() override;
	virtual void OnPositionChanged() override;

	sf::Font* SetFontByPath(const std::string& path);
	sf::Text& GetText() { return m_text; }

	void CenterText();

private:

	void UnregisterSprite();

	int m_entryId = 0;
	sf::Text m_text;
};