#include "VectorMath.hpp"

float VectorModule(sf::Vector2f vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

float VectorArgument(sf::Vector2f vector)
{
	float arg = acos(vector.x / VectorModule(vector));
	if (vector.y < 0) arg = (2 * M_PI) - arg;
	return arg;
}

sf::Vector2f RoundVector(sf::Vector2f vector)
{
	return sf::Vector2f(round(vector.x), round(vector.y));
}