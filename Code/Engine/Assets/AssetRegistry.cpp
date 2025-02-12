#include "AssetRegistry.h"

sf::Texture* AssetRegistry::LoadTexture(const std::string& path)
{
	for (LoadedTexture& loadedTexture : m_loadedTextures)
	{
		if (loadedTexture.identifier == path)
		{
			// found the already loaded texture
			return &loadedTexture.texture;
		}
	}

	sf::Texture newTexture;
	const bool wasSuccessful = newTexture.loadFromFile(path);

	if (wasSuccessful)
	{
		LoadedTexture& loadedTexture = m_loadedTextures.emplace_back();
		loadedTexture.texture = newTexture;
		loadedTexture.identifier = path;

		return &m_loadedTextures.back().texture;
	}

	return nullptr;
}

sf::Font* AssetRegistry::LoadFont(const std::string& path)
{
	for (LoadedFont& loadedFont : m_loadedFonts)
	{
		if (loadedFont.identifier == path)
		{
			// found the already loaded font
			return &loadedFont.font;
		}
	}

	sf::Font newFont;
	const bool wasSuccessful = newFont.loadFromFile(path);

	if (wasSuccessful)
	{
		LoadedFont& loadedFont = m_loadedFonts.emplace_back();
		loadedFont.font = newFont;
		loadedFont.identifier = path;

		return &m_loadedFonts.back().font;
	}

	return nullptr;
}
