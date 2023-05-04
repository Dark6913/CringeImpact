#include "Hitbox.hpp"

void Hitbox::setDamageMultiply(float damage_mult)
{
	m_damage_mult = damage_mult;
}

float Hitbox::getDamageMultiply()
{
	return m_damage_mult;
}

bool Hitbox::isContain(sf::Vector2f point)
{
	sf::FloatRect rect = m_rect;
	if (m_owner)
	{
		rect.left += m_owner->getVisibleBounds().left;
		rect.top += m_owner->getVisibleBounds().top;
	}
	return rect.contains(point);
}

void Hitbox::setOwner(Entity* owner)
{
	m_owner = owner;
}