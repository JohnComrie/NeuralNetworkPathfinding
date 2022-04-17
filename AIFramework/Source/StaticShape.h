#pragma once
#include "box2d/box2d.h"
#include <SFML/Graphics.hpp>
#include "Helpers.h"

namespace AIFramework
{
	class StaticShape : public sf::RectangleShape
	{

	private:
		b2BodyDef bodyDef;
		b2Body* body;
		b2PolygonShape polygonShape;
	public:
		StaticShape(b2World& _world, const sf::Vector2f _position, const float _rotation, const sf::Vector2f _size)
		{
			// Box2d definitions
			bodyDef.position.Set(_position.x / SCALING_FACTOR, _position.y / SCALING_FACTOR);
			bodyDef.angle = _rotation * (PI / 180);
			body = _world.CreateBody(&bodyDef);
			polygonShape.SetAsBox(_size.x / (SCALING_FACTOR * 2), _size.y / (SCALING_FACTOR * 2));
			body->CreateFixture(&polygonShape, 0.0f);

			// SFML definitions
			setSize(_size);
			setOrigin(sf::Vector2f(_size.x / 2, _size.y / 2));
			setFillColor(sf::Color::Blue);
			setPosition(_position);
			setRotation(_rotation);
		}
		void DestroyBody(b2World& _world)
		{
			_world.DestroyBody(body);
		}
	};
}