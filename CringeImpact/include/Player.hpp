#pragma once
#include "VectorMath.hpp"
#include "Entity.hpp"

class Player : public Entity
{
public:
	Player();
	void control(float tick, std::list<Solid*> solid_list);
private:
	bool m_is_walk_sound_plays;
};