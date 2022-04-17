#pragma once
#include "box2d/box2d.h"
#include <SFML/Graphics.hpp>
#include "Helpers.h"

namespace AIFramework
{
	class DynamicShape
	{

	private:
		b2BodyDef bodyDef;

		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;

	public:

		sf::RectangleShape rectangleShape;
		b2Body* body;
		DynamicShape(b2World& _world, const sf::Vector2f _position, const sf::Vector2f _size)
		{
			// Box2d definitions
			bodyDef.position.Set(_position.x / SCALING_FACTOR, _position.y / SCALING_FACTOR);
			bodyDef.type = b2_dynamicBody;
			body = _world.CreateBody(&bodyDef);
			polygonShape.SetAsBox(_size.x / (SCALING_FACTOR * 2), _size.y / (SCALING_FACTOR * 2));

			fixtureDef.shape = &polygonShape;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			body->CreateFixture(&fixtureDef);

			// SFML definitions
			rectangleShape.setSize(_size);
			rectangleShape.setOrigin(sf::Vector2f(_size.x / 2, _size.y / 2));
			rectangleShape.setFillColor(sf::Color::Red);
			rectangleShape.setPosition(sf::Vector2f(_position.x, _position.y));
		}

		void SynchronisePosition();
	};
}