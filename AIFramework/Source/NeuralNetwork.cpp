#include "NeuralNetwork.h"

namespace AIFramework
{
	Neuron::Neuron(int _nInputs) : numInputs(_nInputs + 1)
	{
		for (int i = 0; i < _nInputs + 1; i++)
		{
			// Give neuron random weights
			vecWeights.push_back(RandomFloat());
		}
	}

	Layer::Layer(int _nNeurons, int _nInputsPerNeuron) : numNeurons(_nNeurons)
	{
		for (int i = 0; i < numNeurons; i++)
		{
			// Push new neurons onto the layer
			vecNeurons.push_back(Neuron(_nInputsPerNeuron));
		}
	}

	NeuralNetwork::NeuralNetwork()
	{
		// Create first layer
		vecLayers.push_back(Layer(NEURONS_PER_HIDDEN_LAYER, NUMBER_INPUTS));

		for (int i = 0; i < NUMBER_HIDDEN_LAYERS; i++)
		{
			// Create Hidden layers
			vecLayers.push_back(Layer(NEURONS_PER_HIDDEN_LAYER, NEURONS_PER_HIDDEN_LAYER));
		}

		// Create Output layer
		vecLayers.push_back(Layer(NUMBER_OUTPUTS, NEURONS_PER_HIDDEN_LAYER));
	}

	std::vector<float> NeuralNetwork::GetWeights() const
	{
		// Stack of weights
		std::vector<float> weights;

		// Push weights from  the network onto the new stack
		for (int i = 0; i < NUMBER_HIDDEN_LAYERS + 1; i++)
		{
			for (int j = 0; j < vecLayers[i].numNeurons; j++)
			{
				for (int k = 0; k < vecLayers[i].vecNeurons[j].numInputs; k++)
				{
					weights.push_back(vecLayers[i].vecNeurons[j].vecWeights[k]);
				}
			}
		}

		return weights;
	}

	void NeuralNetwork::ReplaceWeights(const std::vector<float>& _weights)
	{
		int weightCounter = 0;

		// Replace every weight in the network
		for (int i = 0; i < NUMBER_HIDDEN_LAYERS + 1; i++)
		{
			for (int j = 0; j < vecLayers[i].numNeurons; j++)
			{
				for (int k = 0; k < vecLayers[i].vecNeurons[j].numInputs; k++)
				{
					vecLayers[i].vecNeurons[j].vecWeights[k] = _weights[weightCounter++];
				}
			}
		}
		return;
	}

	int NeuralNetwork::GetNumberOfWeights() const
	{
		int weightCounter = 0;

		// Calculate the number of weights in the network
		for (int i = 0; i < NUMBER_HIDDEN_LAYERS + 1; i++)
		{
			for (int j = 0; j < vecLayers[i].numNeurons; j++)
			{
				for (int k = 0; k < vecLayers[i].vecNeurons[j].numInputs; k++)
				{
					weightCounter++;
				}
			}
		}

		return weightCounter;
	}

	std::vector<float> NeuralNetwork::Propagate(std::vector<float>& _inputs)
	{

		std::vector<float> outputs;

		if (_inputs.size() != NUMBER_INPUTS)
		{
			return outputs;
		}

		// For each layer..
		for (int i = 0; i < NUMBER_HIDDEN_LAYERS + 1; i++)
		{
			// If not the first iteration
			if (i > 0)
			{
				_inputs = outputs;
			}
			outputs.clear();

			int weightCounter = 0;

			// For each neuron..
			for (int j = 0; j < vecLayers[i].numNeurons; j++)
			{
				float netInput = 0;
				int numInputs = vecLayers[i].vecNeurons[j].numInputs;

				// For each input..
				for (int k = 0; k < numInputs - 1; k++)
				{
					// Calculate net input..
					// Net input = Sum of input values * input weights
					netInput += vecLayers[i].vecNeurons[j].vecWeights[k] * _inputs[weightCounter++];
				}

				// Add BIAS value to net input
				netInput += vecLayers[i].vecNeurons[j].vecWeights[numInputs - 1] * BIAS;

				// Sigmoid function of net input
				// Clamps value of net input to value between 0 and 1
				// push this onto outputs stack
				outputs.push_back(Sigmoid(netInput));

				weightCounter = 0;
			}
		}
		return outputs;
	}

	void NeuralNetwork::SaveNetworkToFile()
	{
		std::vector<float> weights = GetWeights();

		std::fstream writer("Solution.txt");

		if (writer.is_open())
		{
			for (float weight : weights)
			{
				writer << weight << std::endl;
			}
		}
		writer.close();
	}

	void NeuralNetwork::LoadNetworkFromFile()
	{
		std::fstream reader("Solution.txt");
		std::vector<float> weights;

		if (reader.is_open())
		{
			float tempWeight;
			for (int i = 0; i < GetNumberOfWeights(); i++)
			{
				reader >> tempWeight;
				weights.push_back(tempWeight);
			}
		}
		reader.close();
		ReplaceWeights(weights);
	}
}