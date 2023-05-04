#pragma once
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#define M_7_PI_4		5.49778714378
#define M_3_PI_4		2.35619449019
#define M_5_PI_4		3.92699081699
#define M_3_PI_2		4.71238898038

// Renturns the module(absolute lenght) of input vector
float VectorModule(sf::Vector2f vector);

// Returns the argument(angle) of input vector in radians
float VectorArgument(sf::Vector2f vector);

// Returns vetor with rounded values from input vector
sf::Vector2f RoundVector(sf::Vector2f vector);

// Returns vector with random coordinates within circle described by input params
sf::Vector2f RandomVector(sf::Vector2f center, float radius);

sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b);