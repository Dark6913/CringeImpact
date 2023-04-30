#pragma once
#include "Entity.hpp"

class Enemy : public Entity
{
public:
	Enemy();
	void moveTo(sf::Vector2f point);
	void behave(float tick, std::list<Solid*>& solid_list);
	void setLivingArea(sf::Vector2f point, float radius);
	~Enemy();
private:
	static sf::SoundBuffer* m_death_buffer;
	static sf::SoundBuffer* m_walk_buffer;
	static uint32_t m_instances_count;
	sf::Sound m_death_sound;
	sf::Sound m_walk_sound;
	bool is_have_to_move;
	bool m_is_walk_sound_play;
	float m_move_distance;

	float m_walk_timer;
	float m_walk_cd;
	sf::Vector2f m_living_area_center;
	float m_living_area_radius;
};