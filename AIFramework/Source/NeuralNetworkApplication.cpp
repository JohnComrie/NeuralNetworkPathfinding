#include "NeuralNetworkApplication.h"

namespace AIFramework
{
	void NeuralNetworkApplication::Init()
	{
		// SFML
		window.create(sf::VideoMode(windowWidth, windowHeight), "AIFramework", sf::Style::Default);
		// IMGUI/SFML
		ImGui::SFML::Init(window);

		// Initialise the agents
		for (Agent* agent : agents)
		{
			agent->Initialise(goal.getPosition());
		}
		solutionAgent.Initialise(goal.getPosition());
	}

	void NeuralNetworkApplication::Update()
	{
		deltaTime = deltaClock.restart();
		Input();
		// Update Box2d world
		world->Step(deltaTime.asSeconds(), velocityIterations, positionIterations);
		levelEditor.Update(*world, window);

		// Main loop
		bool generationFinished = true;
		int agentIndex = 0;

		if (showSolutionAgent)
		{
			solutionAgent.Update(*world, deltaTime.asSeconds(), goal.getPosition());
			if (solutionAgent.GetState() == AgentState::Crashed)
			{
				solutionAgent.ResetPosition();
				solutionAgent.SetState(AgentState::Moving);
			}		
		}
		else
		{
			if (simulationStarted)
			{
				for (Agent* agent : agents)
				{
					agent->Update(*world, deltaTime.asSeconds(), goal.getPosition());
					float agentFitness = agent->GetFitness();
					genePopulation[agentIndex++].fitness = agentFitness;
					if (agentFitness > bestFitness)
					{
						bestFitness = agentFitness;
						bestAgent = agent;
					}
					if (agent->GetState() == AgentState::Moving)
						generationFinished = false;
				}

				if (generationFinished || forceNextGeneration)
				{
					forceNextGeneration = false;

					// We replace the solution agent with the best agent this generation if it has a higher fitness value
					if(bestAgent->GetBestFitness() > solutionAgent.GetBestFitness())
					{
						solutionAgent.ReplaceWeights(bestAgent->GetWeights());
					}

					// Get geneeration data for the UI elements
					float averageFitness = 0;
					for (Gene gene : genePopulation)
					{
						averageFitness += gene.fitness;
					}
					averageFitness /= numberOfAgents;
					generationData.push_back(averageFitness);

					// Execute genetic algorithm
					genePopulation = geneticAlgorithm->Epoch(genePopulation);

					// Reset all agents
					agentIndex = 0;
					for (Agent* agent : agents)
					{
						agent->ReplaceWeights(genePopulation[agentIndex++].weights);
						agent->ResetPosition();
						agent->SetState(AgentState::Moving);
					}
				}
			}

		}
		
		UpdateMenus();
		Render();
	}
	void NeuralNetworkApplication::Render()
	{
		window.clear();
		window.draw(goal);

		if (showSolutionAgent)
		{
			solutionAgent.Draw(window);
		}
		else
		{
			// Neural Network Agents
			for (Agent* agent : agents)
			{
				agent->Draw(window);
			}
		}
		// Level editor
		for (StaticShape staticShape : levelEditor.staticShapes)
		{
			window.draw(staticShape);
		}
		levelEditor.RenderGhostLine(window);

		// Render GUI
		ImGui::SFML::Render(window);
		// Render window
		window.display();
	}
	void NeuralNetworkApplication::Input()
	{
		ImGui::SFML::Update(window, deltaTime);
		// Handle any keyboard input here
	}

	void NeuralNetworkApplication::UpdateMenus()
	{
		ImGui::Begin("Neural Network Controls");
		if (showSolutionAgent)
		{
			if (ImGui::Button("Hide Solution Agent")) {
				showSolutionAgent = false;
			}
		}
		else
		{
			if (ImGui::Button("Start")) {
				simulationStarted = true;
				for (Agent* agent : agents)
				{
					agent->SetState(AgentState::Moving);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop")) {
				simulationStarted = false;
				for (Agent* agent : agents)
				{
					agent->SetState(AgentState::Idle);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset")) {
				simulationStarted = false;
				for (Agent* agent : agents)
				{
					agent->ResetPosition();
					agent->SetState(AgentState::Idle);
				}
			}
			if (ImGui::Button("Force Next Generation")) {
				forceNextGeneration = true;
			}
			if (ImGui::Button("Show Solution Agent")) {
				showSolutionAgent = true;
			}
		}
		ImGui::End();


		// Show Neural network data such as average/best fitness
		ImGui::Begin("Neural Network Data");
		ImGui::Text("Best fitness=%f", bestFitness);
		ImGui::Text("Average fitness:");
		for (int i = 0; i < generationData.size(); i++)
		{
			ImGui::BulletText("Generation: %i - %f", i, generationData[i]);
		}
		ImGui::End();
	}
	void NeuralNetworkApplication::Run()
	{
		Init();
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				if (event.type == sf::Event::Closed)
					window.close();
			}
			Update();
		}
	}
}
