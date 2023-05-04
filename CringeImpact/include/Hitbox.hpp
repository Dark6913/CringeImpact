#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "MapObject.hpp"

class Hitbox
{
public:
	Hitbox(sf::Vector2f offset, sf::Vector2f size, float damage_multiply = 1.f, MapObject* owner = NULL) :
		m_damage_mult(damage_multiply),
		m_rect(offset.x, offset.y, size.x, size.y),
		m_owner(owner)
	{}

	// Set multiplyer for incoming damage
	void setDamageMultiply(float damage_mult);

	// Returns damage multioplyer
	float getDamageMultiply() const;

	// Returns hitbox rect position
	sf::Vector2f getOffset() const;

	// Returns hitbox rect size
	sf::Vector2f getSize() const;

	// Check if point inside the hitbox
	bool isContain(sf::Vector2f point) const;

	// Set hitbox owner
	// You have to pass only Entity* in method, i wrote MapObject to avoid file including each other
	void setOwner(MapObject* owner);
private:
	float m_damage_mult;
	sf::FloatRect m_rect;
	MapObject* m_owner;
};