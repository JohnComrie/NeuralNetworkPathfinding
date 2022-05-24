#pragma once

#include "BaseApplication.h"
#include "StaticShape.h"
#include "DynamicShape.h"
#include "Agent.h"
#include "LevelEditor.h"
#include "GeneticAlgorithm.h"

namespace AIFramework
{
	class NeuralNetworkApplication : BaseApplication
	{
		static constexpr int windowWidth = 1600;
		static constexpr int windowHeight = 900;
		static constexpr int numberOfAgents = 30;

		// Functions from BaseApplication
		void Update() override;
		void Render() override;
		void Input() override;

		// Member functions
		void UpdateMenus();

		// Member variables
		LevelEditor levelEditor;

		std::vector<Agent*> agents;
		Agent solutionAgent;
		Agent* bestAgent;

		sf::CircleShape goal;

		float bestFitness;
		std::vector<Gene> genePopulation;
		std::unique_ptr<GeneticAlgorithm> geneticAlgorithm = nullptr;
		std::vector<float> generationData;

		bool simulationStarted = false;
		bool forceNextGeneration = false;
		bool showSolutionAgent = false;

	public:

		// Functions from BaseApplication
		void Init() override;
		void Run() override;

		NeuralNetworkApplication()
			:
			// GameObjects
			goal(sf::CircleShape(10.0f, 30.0f)),
			// Variables
			bestFitness(0),
			solutionAgent(Agent(20.0f, 5)),
			bestAgent(nullptr)
		{
			// Create agents
			for (size_t agentIndex = 0; agentIndex < numberOfAgents; agentIndex++)
			{
				agents.push_back(new Agent(20.0f, 5));
			}

			// Create genetic algorithm
			// Creates a gene for each agent
			geneticAlgorithm = std::make_unique<GeneticAlgorithm>(numberOfAgents, agents[0]->GetNumberOfWeights());
			// Store the gene population in a variable
			genePopulation = geneticAlgorithm->GetChromos();

			int agentIndex = 0;
			for (Agent* agent : agents)
			{
				agent->ReplaceWeights(genePopulation[agentIndex++].weights);
			}
			// Initialise gameobjects
			goal.setFillColor(sf::Color::Green);
			goal.setPosition(sf::Vector2f(windowWidth - 50.0f, 440.0f));
		}
		~NeuralNetworkApplication() 
		{
			for (auto agent : agents)
			{
				delete agent;
			}
			agents.clear();
			delete bestAgent;
		}
	};
}