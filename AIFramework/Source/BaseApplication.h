#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "box2d/box2d.h"
#include <memory>

namespace AIFramework
{
	class BaseApplication
	{
	private:
		const b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
	protected:

		// Pure Virtual Functions
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Input() = 0;
		virtual void Run() = 0;

		// Utility Objects
		sf::View camera;
		sf::Clock deltaClock;
		sf::Time deltaTime;
		sf::Clock timer;

	public:

		// Constructor
		BaseApplication()
			: world(std::make_unique<b2World>(gravity)) 
		{
		};

		// Destructor
		virtual ~BaseApplication() {};

		sf::RenderWindow window;
		std::unique_ptr<b2World> world;
		const uint8_t velocityIterations = 6;
		const uint8_t positionIterations = 2;
	};
}