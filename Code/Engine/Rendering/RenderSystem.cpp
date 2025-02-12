#include "RenderSystem.h"

#include "Engine/Engine.h"

void RenderSystem::Update()
{
	sf::RenderWindow& renderWindow = Engine::GetInstance()->GetRenderWindow();
	sf::RenderStates renderStates;

	for (const ViewEntry& viewEntry : m_views)
	{
		renderWindow.setView(viewEntry.view);
		
		for (const RenderEntry& renderEntry : m_renderEntries)
		{
			renderWindow.draw(*renderEntry.pDrawable, *renderEntry.pTransform);
		}
	}
}

int RenderSystem::AddDrawable(const sf::Drawable* pDrawable, const sf::Transform* pTransform)
{
	RenderEntry& entry = m_renderEntries.emplace_back();
	entry.pDrawable = pDrawable;
	entry.pTransform = pTransform;
	entry.id = ++m_idCounter;

	return entry.id;
}

void RenderSystem::RemoveDrawable(int id)
{
	for (int i = 0; i < m_renderEntries.size(); i++)
	{
		if (m_renderEntries[i].id == id)
		{
			m_renderEntries.erase(m_renderEntries.begin() + i);
			return;
		}
	}
}

int RenderSystem::AddView(const sf::View& view)
{
	ViewEntry& entry = m_views.emplace_back();
	entry.id = ++m_idCounter;
	entry.view = view;

	return entry.id;
}

void RenderSystem::RemoveView(int id)
{
	for (int i = 0; i < m_views.size(); i++)
	{
		if (m_views[i].id == id)
		{
			m_views.erase(m_views.begin() + i);
			return;
		}
	}
}

void RenderSystem::UpdateViewPosition(int id, sf::Vector2f newPosition)
{
	for (int i = 0; i < m_views.size(); i++)
	{
		if (m_views[i].id == id)
		{
			m_views[i].view.setCenter(newPosition);
			return;
		}
	}
}

