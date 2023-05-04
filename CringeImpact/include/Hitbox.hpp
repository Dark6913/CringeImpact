#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "Entity.hpp"

class Hitbox
{
public:
	Hitbox(sf::Vector2f offset, sf::Vector2f size, float damage_multiply = 1.f, Entity* owner = NULL) :
		m_damage_mult(damage_multiply),
		m_rect(offset.x, offset.y, size.x, size.y),
		m_owner(owner)
	{}

	// Set multiplyer for incoming damage
	void setDamageMultiply(float damage_mult);

	// Returns damage multioplyer
	float getDamageMultiply();

	// Check if point inside the hitbox
	bool isContain(sf::Vector2f point);

	// Set hitbox owner
	void setOwner(Entity* owner);
private:
	float m_damage_mult;
	sf::FloatRect m_rect;
	Entity* m_owner;
};