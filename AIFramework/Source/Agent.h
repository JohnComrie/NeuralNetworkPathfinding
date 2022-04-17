#pragma once
#include <SFML/Graphics.hpp>
#include "RayCastLine.h"
#include "NeuralNetwork.h"

namespace AIFramework
{
	enum class AgentState {
		Idle, Moving, Crashed
	};

	class Agent : public NeuralNetwork
	{
	public:
		explicit Agent(const float _size, const uint8_t _numRays)
			: triangle(sf::CircleShape(_size, 3)),
			numRays(_numRays),
			fitness(0),
			bestFitness(0),
			startDistance(0),
			baseMovementSpeed(500.0f),
			baseRotationSpeed(1000.0f),
			startPosition(sf::Vector2f(100.0f, 450.0f)),
			state(AgentState::Moving)
		{
			triangle.setFillColor(sf::Color::Red);
			triangle.setOrigin(_size, _size);
			triangle.setPosition(startPosition);
			triangle.setRotation(90.0f);
			rays.reserve(_numRays);
			for (uint8_t i = 0; i < _numRays; i++)
			{
				rays.emplace_back(RayCastLine(sf::Vector2f(0.0f, 0.0f), 0.0f, 100.0f));
			}
		}

		void Initialise(const sf::Vector2f goalPosition);
		void Update(b2World& _world, const float _deltaTime, const sf::Vector2f _goalPosition);
		void Draw(sf::RenderWindow& _window);
		void ResetPosition();

		// Getters/Setters
		sf::CircleShape GetTriangle() const;
		float GetFitness() const;
		float GetBestFitness() const;
		AgentState GetState() const;
		void SetState(const AgentState _state);
	private:
		sf::CircleShape triangle;
		uint8_t numRays;
		std::vector<RayCastLine> rays;
		AgentState state;

		// Neural Network variables
		float fitness;
		float bestFitness;
		float startDistance;

		// Misc variables
		float baseMovementSpeed;
		float baseRotationSpeed;
		sf::Vector2f startPosition;
		sf::Vector2f goalPosition;
		
		// Member functions
		void Rotate(const float _angle);
		void Move(const float _distance);
		void CalculateFitness();
	};
}
