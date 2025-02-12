#include "ViewComponent.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/RenderSystem.h"

#include "Engine/EntitySystem/ComponentFactory.h"

void ViewComponent::Shutdown()
{
	UnregisterView();
}

void ViewComponent::OnPositionChanged()
{
	if (m_viewId != 0)
	{
		Engine::GetInstance()->GetRenderSystem().UpdateViewPosition(m_viewId, GetOwner()->GetPosition());
	}
}

void ViewComponent::SetView(const sf::View& view)
{
	UnregisterView();

	m_viewId = Engine::GetInstance()->GetRenderSystem().AddView(view);
}

void ViewComponent::UnregisterView()
{
	if (m_viewId != 0)
	{
		Engine::GetInstance()->GetRenderSystem().RemoveView(m_viewId);
		m_viewId = 0;
	}
}

REGISTER_COMPONENT_FACTORY(ViewComponent)