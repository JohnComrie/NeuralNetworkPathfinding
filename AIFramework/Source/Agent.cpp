#include "Agent.h"
#include <iostream>
#include <cmath>
#include "Helpers.h"

namespace AIFramework
{
	void Agent::Initialise(const sf::Vector2f _goalPosition)
	{
		goalPosition = _goalPosition;
		startDistance = Distance(triangle.getPosition(), goalPosition);
	}
	void Agent::CalculateFitness()
	{
		fitness = (1 - (Distance(triangle.getPosition(), goalPosition) / startDistance)) * 100;
		if (fitness > bestFitness)
			bestFitness = fitness;
	}
	void Agent::Draw(sf::RenderWindow& _window)
	{
		_window.draw(triangle);
		if (state == AgentState::Moving)
		{
			for (RayCastLine ray : rays)
			{
				_window.draw(ray);
			}
		}
	}
	void Agent::Rotate(const float _angle)
	{
		triangle.setRotation(triangle.getRotation() + _angle);
	}

	void Agent::Move(const float _distance)
	{
		sf::Vector2f forwardVector;
		
		forwardVector.x = cos((triangle.getRotation() - 90) * (PI / 180));
		forwardVector.y = sin((triangle.getRotation() - 90) * (PI / 180));

		triangle.setPosition(triangle.getPosition() + (forwardVector * _distance));
	}
	void Agent::ResetPosition()
	{
		triangle.setRotation(90.0f);
		triangle.setPosition(startPosition);
	}
	sf::CircleShape Agent::GetTriangle() const
	{
		return triangle;
	}
	float Agent::GetFitness() const
	{
		return fitness;
	}
	float Agent::GetBestFitness() const
	{
		return bestFitness;
	}
	AgentState Agent::GetState() const
	{
		return state;
	}
	void Agent::SetState(const AgentState _state)
	{
		state = _state;
	}
	void Agent::Update(b2World& _world, const float deltaTime, const sf::Vector2f goalPosition)
	{
		if (state != AgentState::Moving)
			return;

		float rotation = triangle.getRotation() - 90;

		// Create vector of inputs to hand to the neural network
		std::vector<float> inputs;
		inputs.reserve(numRays);

		for (RayCastLine& ray : rays)
		{
			float rayFraction = ray.CalculateFraction(_world, triangle.getPosition(), rotation);
			rotation += 180 / (numRays - 1);

			// Each ray fraction will be an input for the neural network
			// Ray fraction is number between 0 and 1, depending on where the ray intersects an object
			inputs.emplace_back(rayFraction);

			if (rayFraction < 0.1)
			{
				state = AgentState::Crashed;
			}
		}

		// Propogate inputs through the neural network to get outputs
		std::vector<float> outputs = Propagate(inputs);

		// Movement speed is calculated based on NN output 1
		// Number between 0 and 1 multiplied by base speed
		Move(baseMovementSpeed * outputs[0] * deltaTime);

		// Convert rotation output to be between -1 and 1 so the agent can rotate both left and right
		// Multiply base rotation speed by NN output 2
		float rotationOutput = (outputs[1] * 2.0f) - 1.0f;
		Rotate(baseRotationSpeed * rotationOutput * deltaTime);

		CalculateFitness();
	}
}