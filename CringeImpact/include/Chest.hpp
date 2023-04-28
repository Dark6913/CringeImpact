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
	~Chest();
private:
	static sf::SoundBuffer* m_sound_buffer;
	static uint32_t m_instances_count;
	Animation m_animation;
	sf::Sound m_sound;
	bool m_is_open;
	bool m_was_open;
};