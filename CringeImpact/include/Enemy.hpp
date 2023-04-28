#pragma once
#include "Entity.hpp"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
private:
	static sf::SoundBuffer* m_death_buffer;
	static uint32_t m_instances_count;
	sf::Sound m_death_sound;
};