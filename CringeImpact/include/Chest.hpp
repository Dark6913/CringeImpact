#pragma once
#include "VectorMath.hpp"
#include "MapObject.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Chest : public MapObject
{
public:
	Chest();
	void open();
	void close();
	void toggle();
	void update(float tick) override;
private:
	Animation m_animation;
	bool m_is_open;
	bool m_was_open;

	sf::Sound* m_open_sound_ptr;
	sf::Sound* m_close_sound_ptr;
};