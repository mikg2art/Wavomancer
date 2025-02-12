#pragma once

#include "Engine/EntitySystem/EntitySystemDefinitions.h"

class ViewComponent : public EntityComponent
{
public:

	virtual void Shutdown() override;
	virtual void OnPositionChanged() override;

	void SetView(const sf::View& view);

private:

	void UnregisterView();

	int m_viewId = 0;

};