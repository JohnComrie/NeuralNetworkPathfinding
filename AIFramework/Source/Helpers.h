#pragma once

#define SCALING_FACTOR 10
#define PI 3.14159265359

#include "SFML/Graphics.hpp"

// Returns number between 0.0f and 1.0f
inline float RandomFloatUnsigned() {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
}
// Returns number between -1.0f and 1.0f
inline float RandomFloat() {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2)) - 1.0f;
}
// Calculate distance between two points
inline float Distance(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}