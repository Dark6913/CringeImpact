#pragma once
#include <SFML/Graphics.hpp>

class IAnimated : public sf::Drawable
{
public:
	virtual void update(float tick) = 0;
	virtual sf::FloatRect getVisibleBounds() const = 0;
	virtual sf::Vector2f getCenter() const = 0;
};