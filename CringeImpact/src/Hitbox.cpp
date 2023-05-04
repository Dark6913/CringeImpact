#include "Hitbox.hpp"

void Hitbox::setDamageMultiply(float damage_mult)
{
	m_damage_mult = damage_mult;
}

float Hitbox::getDamageMultiply() const
{
	return m_damage_mult;
}

bool Hitbox::isContain(sf::Vector2f point) const
{
	sf::FloatRect rect = m_rect;
	if (m_owner)
	{
		rect.left += m_owner->getVisibleBounds().left;
		rect.top += m_owner->getVisibleBounds().top;
	}
	return rect.contains(point);
}

sf::Vector2f Hitbox::getOffset() const
{
	return sf::Vector2f(m_rect.left, m_rect.top);
}

sf::Vector2f Hitbox::getSize() const
{
	return sf::Vector2f(m_rect.width, m_rect.height);
}

void Hitbox::setOwner(MapObject* owner)
{
	m_owner = owner;
}