#pragma once

#include <SFML/Graphics.hpp>

#include <list>

struct LoadedTexture
{
	std::string identifier;
	sf::Texture texture;
};

struct LoadedFont
{
	std::string identifier;
	sf::Font font;
	int userCount = 0;
};

class AssetRegistry
{
public:

	sf::Texture* LoadTexture(const std::string& path);
	sf::Font* LoadFont(const std::string& path);

private:

	std::list<LoadedTexture> m_loadedTextures;
	std::list<LoadedFont> m_loadedFonts;

};