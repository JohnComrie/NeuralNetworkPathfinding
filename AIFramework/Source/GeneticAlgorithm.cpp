#include "GeneticAlgorithm.h"

namespace AIFramework
{
	GeneticAlgorithm::GeneticAlgorithm(int _populationSize, int _numWeights) 
		: populationSize(_populationSize),
		geneLength(_numWeights),
		totalFitness(0),
		bestFitness(0),
		averageFitness(0)
	{

		for (int i = 0; i < _populationSize; ++i)
		{
			// Populate vector of Genes
			vecPopulation.push_back(Gene());
			for (int j = 0; j < geneLength; ++j)
			{
				// Give Population random weights
				vecPopulation[i].weights.push_back(RandomFloat());
			}
		}
	}

	void GeneticAlgorithm::Mutate(std::vector<float>& _gene)
	{
		for (int i = 0; i < _gene.size(); ++i)
		{
			// Determine whether this gene should be mutated
			if (RandomFloatUnsigned() < MUTATION_RATE)
			{
				//add or subtract a small value to the weight
				_gene[i] += (RandomFloat() * MAX_WEIGHT_CHANGE);
			}
		}
	}

	Gene GeneticAlgorithm::CrossOver(Gene _parentA, Gene _parentB)
	{
		// Create child object
		Gene child;

		// Generate random midpoint for crossover
		int midpoint = int(rand() % _parentA.weights.size());

		// Determine which parent leads the new child object
		if ((rand() % 2 == 0))
		{
			// Crossover
			// Parent B - Parent A
			for (int i = 0; i < _parentA.weights.size(); i++)
			{
				if (i > midpoint) child.weights.push_back(_parentA.weights[i]);
				else child.weights.push_back(_parentB.weights[i]);
			}
		}
		else {
			// Crossover
			// Parent A - Parent B
			for (int i = 0; i < _parentA.weights.size(); i++)
			{
				if (i > midpoint) child.weights.push_back(_parentB.weights[i]);
				else child.weights.push_back(_parentA.weights[i]);
			}
		}
		return child;
	}

	std::vector<Gene> GeneticAlgorithm::Epoch(const std::vector<Gene>& _old_pop)
	{
		vecPopulation = _old_pop;
		Reset();
		CalculateData();

		// PUSH POPULATION ONTO MATING POOL
		for (int j = 0; j < vecPopulation.size(); j++)
		{
			int n = int(vecPopulation[j].fitness);
			for (int k = 0; k < n; k++)
			{
				matingPool.push_back(vecPopulation[j]);
			}
		}

		// CROSS OVER RANDOM GENES TO CREATE CHILD GENE
		for (int j = 0; j < vecPopulation.size(); j++)
		{
			int a = rand() % matingPool.size();
			int b = rand() % matingPool.size();

			Gene parentA = matingPool[a];
			Gene parentB = matingPool[b];

			Gene child;
			if (RandomFloatUnsigned() < CROSSOVER_RATE)
			{
				child = CrossOver(parentA, parentB);
			}
			else
			{
				if ((rand() % 2 == 0))
					child = parentA;
				else
					child = parentB;
			}
			Mutate(child.weights);

			vecPopulation[j] = child;
		}

		return vecPopulation;

	}

	void GeneticAlgorithm::CalculateData()
	{
		totalFitness = 0;
		float HighestSoFar = 0;

		for (int i = 0; i < populationSize; ++i)
		{
			// update fittest if necessary
			if (vecPopulation[i].fitness > HighestSoFar)
			{
				HighestSoFar = vecPopulation[i].fitness;
				bestFitness = HighestSoFar;
			}

			totalFitness += vecPopulation[i].fitness;
		}
	}

	void GeneticAlgorithm::Reset()
	{
		totalFitness = 0;
		bestFitness = 0;
		averageFitness = 0;
	}
}