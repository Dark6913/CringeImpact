#pragma once
#include "Entity.hpp"

class Enemy : public Entity
{
public:
	Enemy();
	void moveTo(sf::Vector2f point);
	void behave(float tick);
	~Enemy();
private:
	static sf::SoundBuffer* m_death_buffer;
	static uint32_t m_instances_count;
	sf::Sound m_death_sound;
	bool is_have_to_move;
	float m_move_distance;
};