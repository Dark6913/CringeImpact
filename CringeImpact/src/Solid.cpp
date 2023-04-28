#include "Solid.hpp"

bool Solid::isIntersects(Solid* another)
{
	return this->isMovedIntersects(another, sf::Vector2f(0, 0));
}

bool Solid::isMovedIntersects(Solid* another, sf::Vector2f delta)
{
	for (auto it1 = m_collisions.begin(); it1 != m_collisions.end(); it1++)
	{
		for (auto it2 = another->m_collisions.begin(); it2 != another->m_collisions.end(); it2++)
		{
			sf::FloatRect moved_rect = (sf::FloatRect)*it1;
			moved_rect.left += m_object_position.x - m_object_texture_origin_offset.x;
			moved_rect.top += m_object_position.y - m_object_texture_origin_offset.y;
			if (delta.x <= 0)
			{
				moved_rect.left += delta.x;
				moved_rect.width -= delta.x;
			}
			else moved_rect.width += delta.x;
			if (delta.y <= 0)
			{
				moved_rect.top += delta.y;
				moved_rect.height -= delta.y;
			}
			else moved_rect.height += delta.y;

			sf::FloatRect another_rect = (sf::FloatRect)*it2;
			another_rect.left += another->m_object_position.x - another->m_object_texture_origin_offset.x;
			another_rect.top += another->m_object_position.y - another->m_object_texture_origin_offset.y;

			if (moved_rect.intersects(another_rect)) return true;
		}
	}
	return false;
}

void Solid::setAbsoluteObjectPosition(sf::Vector2f position)
{
	m_object_position = position;
}

void Solid::setObjectTextureOriginOffset(sf::Vector2f offset)
{
	m_object_texture_origin_offset = offset;
}

void Solid::displayCollision(sf::RenderTarget& target) const
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setOutlineColor(sf::Color(255, 255, 255));
	rect.setOutlineThickness(-1);
	for (auto it = m_collisions.begin(); it != m_collisions.end(); it++)
	{
		rect.setPosition(
			m_object_position.x - m_object_texture_origin_offset.x + it->left,
			m_object_position.y - m_object_texture_origin_offset.y + it->top
		);
		rect.setSize(sf::Vector2f(it->width, it->height));
		target.draw(rect);
	}
}