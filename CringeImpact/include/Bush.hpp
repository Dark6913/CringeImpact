#pragma once
#include "MapObject.hpp"

class Bush : public MapObject
{
public:
	Bush();
private:
	Animation m_animation;
};