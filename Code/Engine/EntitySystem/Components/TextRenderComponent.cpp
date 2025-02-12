#include "TextRenderComponent.h"
#include "Engine/Engine.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Rendering/RenderSystem.h"

#include "Engine/EntitySystem/ComponentFactory.h"

void TextRenderComponent::Shutdown()
{
	UnregisterSprite();
}

void TextRenderComponent::OnPositionChanged()
{
	//Updates the cached transform that is pointed at by the render system
	GetOwner()->GetTransform(); 
}

sf::Font* TextRenderComponent::SetFontByPath(const std::string& path)
{
	UnregisterSprite();

	if (sf::Font* pFontPtr = Engine::GetInstance()->GetAssetRegistry().LoadFont(path))
	{
		m_text.setFont(*pFontPtr);

		m_entryId = Engine::GetInstance()->GetRenderSystem().AddDrawable(&m_text, &GetOwner()->GetTransform());

		return pFontPtr;
	}

	return nullptr;
}

void TextRenderComponent::CenterText()
{
	m_text.setOrigin(m_text.getGlobalBounds().getSize() / 2.f + m_text.getLocalBounds().getPosition());
}

void TextRenderComponent::UnregisterSprite()
{
	if (m_entryId != 0)
	{
		Engine::GetInstance()->GetRenderSystem().RemoveDrawable(m_entryId);
		m_entryId = 0;
	}
}

REGISTER_COMPONENT_FACTORY(TextRenderComponent)