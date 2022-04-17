#pragma once
#include <iostream>
#include "box2d/box2d.h"
#include <SFML/Graphics.hpp>
#include "Helpers.h"

namespace AIFramework
{
	class RayCastLine : public sf::RectangleShape
	{
		b2RayCastInput rayCastInput = b2RayCastInput();
		b2RayCastOutput rayCastOutput = b2RayCastOutput();
		sf::Vector2f origin;
		float angle;
		const float distance;
		float fraction;
	public:
		RayCastLine(sf::Vector2f _origin, float _angle, const float _distance)
			: origin(_origin),
			angle(_angle),
			distance(_distance),
			fraction(0)
		{
			setFillColor(sf::Color::Green);
			setPosition(origin);
			setRotation(angle);
			setSize(sf::Vector2f(distance, 1.0f));
		}
		friend std::ostream& operator<< (std::ostream& _out, const sf::Vector2f& _vec2);
		friend std::ostream& operator<< (std::ostream& _out, const b2Vec2& _vec2);

		// Returns ray fraction
		float CalculateFraction(b2World& _world, const sf::Vector2f _newOrigin, const float _newAngle);
	};
}