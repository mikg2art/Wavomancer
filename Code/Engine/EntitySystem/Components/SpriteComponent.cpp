#include "SpriteComponent.h"
#include "Engine/Engine.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Rendering/RenderSystem.h"

#include "Engine/EntitySystem/ComponentFactory.h"

void SpriteComponent::Shutdown()
{
	UnregisterSprite();
}

void SpriteComponent::OnPositionChanged()
{
	//Updates the cached transform that is pointed at by the render system
	GetOwner()->GetTransform();
}

sf::Texture* SpriteComponent::SetSpriteByPath(const std::string& path, bool centerSprite)
{
	UnregisterSprite();

	if (sf::Texture* pTexturePtr = Engine::GetInstance()->GetAssetRegistry().LoadTexture(path))
	{
		m_sprite.setTexture(*pTexturePtr);

		if (centerSprite)
		{
			sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
			m_sprite.setOrigin(textureSize.x * 0.5f, textureSize.y * 0.5f);
		}

		SetVisibility(true);

		return pTexturePtr;
	}

	return nullptr;
}

void SpriteComponent::SetVisibility(bool isVisible)
{
	if (isVisible == m_isVisible)
		return;

	m_isVisible = isVisible;

	if (m_isVisible)
	{
		m_entryId = Engine::GetInstance()->GetRenderSystem().AddDrawable(&m_sprite, &GetOwner()->GetTransform());
	}
	else
	{
		UnregisterSprite();
	}
}

void SpriteComponent::UnregisterSprite()
{
	if (m_entryId != 0)
	{
		Engine::GetInstance()->GetRenderSystem().RemoveDrawable(m_entryId);
		m_isVisible = false;
		m_entryId = 0;
	}
}

REGISTER_COMPONENT_FACTORY(SpriteComponent)