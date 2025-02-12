#include "HealthbarEntity.h"

#include "Engine/Engine.h"
#include "Engine/EntitySystem/EntitySystem.h"
#include "Engine/EntitySystem/Components/SpriteComponent.h"

void HealthbarEntity::Init()
{
	m_pBackground = AddComponent<SpriteComponent>();
	sf::Texture* pBackgroundTexture = m_pBackground->SetSpriteByPath("../Assets/Custom/health_bg.png", true);

	m_pHealthFill = AddComponent<SpriteComponent>();
	sf::Texture* pFillTexture = m_pHealthFill->SetSpriteByPath("../Assets/Custom/health_fill.png", true);

	const sf::Vector2u bgSize = pBackgroundTexture->getSize();
	const sf::Vector2u fillSize = pFillTexture->getSize();

	if (bgSize.x >= fillSize.x && bgSize.y >= fillSize.y)
	{
		sf::Vector2f diff = sf::Vector2f(static_cast<float>(bgSize.x - fillSize.x), static_cast<float>(bgSize.y - fillSize.y)) * 0.5f;

		m_pHealthFill->GetSprite().setOrigin(0.f, 0.f);
		m_pHealthFill->GetSprite().setPosition(sf::Vector2f(bgSize.x * -0.5f + diff.x, bgSize.y * -0.5f + diff.y));
		//UpdateHealthColor();
	}
	else
	{
		std::cout << "Invalid healthbar texture setup! Background needs to be bigger than fill." << std::endl;
	}
}

void HealthbarEntity::Update(float deltaSeconds)
{
	if (Entity* pOwner = Engine::GetInstance()->GetEntitySystem().FindEntityById(m_ownerId))
	{
		if (IDamageable* pDamageable = dynamic_cast<IDamageable*>(pOwner))
		{
			const float ratio = pDamageable->GetHealth() / pDamageable->GetMaxHealth();
			m_pHealthFill->GetSprite().setScale(sf::Vector2f(ratio, 1.f));
		}

		//UpdateHealthColor();
		SetPosition(pOwner->GetPosition() + m_offset);
	}
}

void HealthbarEntity::SetOwnerEntity(int ownerId)
{
	m_ownerId = ownerId;
}

void HealthbarEntity::SetOffset(sf::Vector2f offset)
{
	m_offset = offset;
}

void HealthbarEntity::UpdateHealthColor()
{
	if (Entity* pOwner = Engine::GetInstance()->GetEntitySystem().FindEntityById(m_ownerId))
	{
		if (IDamageable* pDamageable = dynamic_cast<IDamageable*>(pOwner))
		{
			const float ratio = pDamageable->GetHealth() / pDamageable->GetMaxHealth();
			const int R = (ratio * 255);
			const int G = 255 - (ratio * 255);
			const int B = 0;
			sf::Color color(R, G, B);
			m_pHealthFill->GetSprite().setColor(color);
		}
	}
}
