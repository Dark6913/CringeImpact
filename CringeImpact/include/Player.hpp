#pragma once
#include "VectorMath.hpp"
#include "Entity.hpp"
#include "World.hpp"

class Player : public Entity
{
public:
	Player();
	void control(float tick, std::list<Solid*> solid_list);
private:
	bool m_is_walk_sound_plays;
	World::SurfaceType m_last_st; // Last surface type to check if have to create new sound instance
};