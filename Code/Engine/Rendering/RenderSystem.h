#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct RenderEntry
{
	int id = 0;
	const sf::Transform* pTransform = nullptr;
	const sf::Drawable* pDrawable = nullptr;
};

struct ViewEntry
{
	int id = 0;
	sf::View view;
};

class RenderSystem
{
public:

	void Update();

	int AddDrawable(const sf::Drawable* pDrawable, const sf::Transform* pTransform);
	void RemoveDrawable(int id);

	int AddView(const sf::View& view);
	void RemoveView(int id);

	void UpdateViewPosition(int id, sf::Vector2f newPosition);

private:

	std::vector<RenderEntry> m_renderEntries;
	std::vector<ViewEntry> m_views;

	int m_idCounter = 0;

};