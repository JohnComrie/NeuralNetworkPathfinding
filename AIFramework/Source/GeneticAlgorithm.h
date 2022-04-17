#pragma once
#include <vector>
#include "Helpers.h"
#include <iostream>
#include <fstream>

#define MUTATION_RATE 0.1
#define CROSSOVER_RATE 1.0
#define MAX_WEIGHT_CHANGE 0.5

namespace AIFramework
{
	struct Gene
	{
		// Stack of weights
		std::vector <float> weights;

		// Fitness value
		float fitness;

		// Constructors
		Gene() 
			: fitness(0) 
		{}
		Gene(const std::vector <float>& _weights, const float _fitness) 
			: weights(_weights),
			fitness(_fitness)
		{}

		// Override function
		friend bool operator<(const Gene& _lhs, const Gene& _rhs)
		{
			return (_lhs.fitness < _rhs.fitness);
		}
	};

	class GeneticAlgorithm
	{
	private:

		// Stack of Genes for Cross production
		std::vector<Gene> matingPool;

		// Stack for entire population
		std::vector<Gene> vecPopulation;

		// size of population
		int populationSize;
		// amount of weights per Gene
		int geneLength;

		float totalFitness;
		float bestFitness;
		float averageFitness;

		// Functions
		Gene CrossOver(Gene _parentA, Gene _parentB);
		void Mutate(std::vector<float>& _chromo);
		void CalculateData();
		void Reset();

	public:
		GeneticAlgorithm(int _populationSize, int _numWeights);

		// Epoch runs the GA for a generation
		std::vector<Gene> Epoch(const std::vector<Gene>& _old_pop);

		// Getters
		std::vector<Gene> GetChromos() const { return vecPopulation; }
		float AverageFitness() const { return totalFitness / populationSize; }
		float BestFitness() const { return bestFitness; }
	};
}