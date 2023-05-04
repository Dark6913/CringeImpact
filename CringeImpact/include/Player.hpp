#pragma once
#include "VectorMath.hpp"
#include "Entity.hpp"

class Player : public Entity
{
public:
	Player();
	void control(float tick, std::list<Solid*> solid_list);
	~Player();
private:
	static sf::SoundBuffer* m_walk_buffer;
	static sf::SoundBuffer* m_attack_buffer;
	static uint32_t m_instances_count;
	bool m_is_walk_sound_plays;
	sf::Sound m_walk_sound;
	sf::Sound m_attack_sound;
};