#pragma once
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#define M_7_PI_4		5.49778714378
#define M_3_PI_4		2.35619449019
#define M_5_PI_4		3.92699081699
#define M_3_PI_2		4.71238898038

float VectorModule(sf::Vector2f vector);
float VectorArgument(sf::Vector2f vector);
sf::Vector2f RoundVector(sf::Vector2f vector);