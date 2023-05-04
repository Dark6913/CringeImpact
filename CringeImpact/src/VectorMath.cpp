#include "VectorMath.hpp"
#include <random>

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

sf::Vector2f RandomVector(sf::Vector2f center, float radius)
{
	srand(time(0) - rand());
	float angle = (float)((rand() + rand()) % 360) / 180.f * M_PI;
	float distance = (float)((rand() + rand()) % (int)radius);
	return center + sf::Vector2f(distance * cos(angle), distance * sin(angle));
}

sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b)
{
	return sf::Vector2f(a.x * b.x, a.y * b.y);
}