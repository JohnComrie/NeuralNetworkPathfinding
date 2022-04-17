#include "RayCastLine.h"

namespace AIFramework
{
	std::ostream& operator<< (std::ostream& _out, const sf::Vector2f& _vec2)
	{
		_out << _vec2.x << ", " << _vec2.y;
		return _out;
	}

	std::ostream& operator<< (std::ostream& _out, const b2Vec2& _vec2)
	{
		_out << _vec2.x * SCALING_FACTOR << ", " << _vec2.y * SCALING_FACTOR;
		return _out;
	}

	// Returns float between 0.0f and 1.0f
	// Smaller number = hit object is closer
	// Number == 1.0f = no hit
	float RayCastLine::CalculateFraction(b2World& _world, const sf::Vector2f _newOrigin, const float _newAngle)
	{
		rayCastInput.p1 = b2Vec2(_newOrigin.x / SCALING_FACTOR, _newOrigin.y / SCALING_FACTOR);
		// Calculate direction of ray using angle
		// Calculate second point by multipling direction vector by distance
		// Include scaling factor in calculation to ensure consistent translation from pixels to box2d metres
		rayCastInput.p2 = rayCastInput.p1 + (distance / SCALING_FACTOR) * b2Vec2(cos((_newAngle - 90) * PI / 180), sin((_newAngle - 90) * PI / 180));
		rayCastInput.maxFraction = 1;

		// Update line
		setRotation(_newAngle + 180.0f); // add 180 degrees because box2d y-axis is opposite of SFML
		setPosition(_newOrigin);
		setSize(sf::Vector2f(1.0f, distance));
		float closestFraction = rayCastInput.maxFraction;

		for (b2Body* body = _world.GetBodyList(); body; body = body->GetNext()) {
			for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
				if (!fixture->RayCast(&rayCastOutput, rayCastInput, 0))
				{
					continue;
				}
				if (rayCastOutput.fraction < closestFraction)
				{
					closestFraction = rayCastOutput.fraction;
					
					setSize(sf::Vector2f(1.0f, rayCastOutput.fraction * distance));
				}
			}
		}

		return closestFraction;
	}
}