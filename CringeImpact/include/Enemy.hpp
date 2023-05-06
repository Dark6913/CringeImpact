#pragma once
#include "Entity.hpp"

class Enemy : public Entity
{
public:
	Enemy();
	void moveTo(sf::Vector2f point);
	void behave(float tick, std::list<Entity*> players_list, std::list<Solid*>& solid_list);
	void setLivingArea(sf::Vector2f point, float radius);
	void setTarget(Entity* target);
private:
	// Stop an enemy
	void stopMoving();

	bool is_have_to_move;
	bool m_is_walk_sound_play;
	float m_move_distance;

	float m_walk_timer;
	float m_walk_cd;
	sf::Vector2f m_living_area_center;
	float m_living_area_radius;

	float m_agressive_range;
	float m_escape_distance;
	float m_calm_range;
	float m_living_area_extra_range;

	Entity* m_target;
};