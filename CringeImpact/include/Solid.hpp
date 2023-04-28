#pragma once
#include <SFML/Graphics.hpp>
#include <list>

class Solid
{
public:
	Solid() : m_object_position({ 0, 0 }), m_object_texture_origin_offset({ 0, 0 }) {};
	bool isIntersects(Solid* another);
	bool isMovedIntersects(Solid* another, sf::Vector2f delta);
protected:
	void setAbsoluteObjectPosition(sf::Vector2f position);
	void setObjectTextureOriginOffset(sf::Vector2f offset);
	std::list<sf::IntRect> m_collisions;
	void displayCollision(sf::RenderTarget& target) const;
private:
	sf::Vector2f m_object_position;
	sf::Vector2f m_object_texture_origin_offset;
};