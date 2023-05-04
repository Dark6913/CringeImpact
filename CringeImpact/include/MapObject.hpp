#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Solid.hpp"
#include "Animation.hpp"
#include "IAnimated.hpp"

class MapObject : public IAnimated, public Solid
{
public:
	MapObject();
	void setPosition(sf::Vector2f position);
	void setListenerPosition(sf::Vector2f listener_pos);
	void loadAnimation(sf::Texture& texture, uint8_t frames_count, uint8_t rows_count,
		float frame_duration, sf::Vector2f scale = { 1.f, 1.f }, sf::Vector2f origin = {0.f, 0.f});
	void loadAnimation(std::string file, uint8_t frames_count, uint8_t rows_count,
		float frame_duration, sf::Vector2f scale = { 1.f, 1.f }, sf::Vector2f origin = { 0.f, 0.f });
	void addCollisionRect(sf::IntRect rect);
	sf::Vector2f getPosition();
	sf::FloatRect getVisibleBounds() const override;
	sf::Vector2f getCenter() const override;
	virtual void update(float tick) override;
protected:
	// Set the current animation and texture origin offset for collsion boxes
	// This function have to be used always when you want to change current animation
	void setCurrentAnimation(Animation* anim);

	sf::Vector2f m_position;
	Animation* m_animation_ptr;
	sf::Vector2f m_listener_position;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};