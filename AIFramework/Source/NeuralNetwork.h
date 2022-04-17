#pragma once
#include <vector>
#include "Helpers.h"
#include <fstream>

#define NUMBER_INPUTS 5
#define NUMBER_HIDDEN_LAYERS 1
#define NEURONS_PER_HIDDEN_LAYER 5
#define NUMBER_OUTPUTS 2
#define BIAS -1
#define ACIVATION_RESPONSE 1

namespace AIFramework
{

	// Struct to hold neuron data
	struct Neuron
	{
		// Number of inputs
		int numInputs;
		// Stack of weights
		std::vector<float> vecWeights;

		// Constructor
		explicit Neuron(int _nInputs);
	};

	struct Layer
	{
		// Number of neurons
		int numNeurons;
		// Stack of neurons
		std::vector<Neuron> vecNeurons;

		// Constructor
		Layer(int _nNeurons, int _nInputsPerNeuron);
	};

	class NeuralNetwork {

	private:

		// Stack of layers
		std::vector<Layer> vecLayers;

		// Activation functions
		inline float Sigmoid(const float _activation) const {
			return (1 / (1 + exp(-_activation)));
		}
		// Alternative activation function
		inline float TanH(const float _activation) const {
			return ((exp(_activation) - exp(-_activation)) / exp(_activation) + exp(-_activation));
		}

	public:

		// Constructors
		NeuralNetwork();

		// Returns the stack of weights
		std::vector<float> GetWeights() const;
		// Returns the number of weights
		int GetNumberOfWeights() const;
		// Function to replace weights in the entire network
		void ReplaceWeights(const std::vector<float>& _weights);

		// Update function
		std::vector<float> Propagate(std::vector<float>& _inputs);

		void SaveNetworkToFile();
		void LoadNetworkFromFile();
	};
}