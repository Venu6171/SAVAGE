#include "Precompiled.h"
#include "NeuralNetwork.h"
#include "Datasets.h"

using namespace SAVAGE::ML;
using namespace SAVAGE::Math;

namespace
{
	// This works better for XOR
	float TanH(float x)
	{
		return tanh(x);
	}

	float TanHDerivate(float x)
	{
		return 1.0f - (x * x);
	}

	// This works better for Digit
	float Sigmoid(float x)
	{
		return 1 / (1 + exp(-x));
	}

	float SigmoidDerivative(float x)
	{
		float out = Sigmoid(x);
		return out * (1 - out);
	}
}

Neuron::Neuron(size_t numOutputs, size_t myIndex)
	: mMyIndex(myIndex)
{
	for (size_t i = 0; i < numOutputs; ++i)
	{
		mOutPutWeights.push_back({ RandomFloat() });
	}
}

void Neuron::FeedForward(const Layer& previousLayer, std::string functionName)
{
	float sum = 0.0f;

	for (size_t neuron = 0; neuron < previousLayer.size(); ++neuron)
	{
		auto& n = previousLayer[neuron];
		sum += n.GetOutputValue() * n.mOutPutWeights[mMyIndex].weight;
	}

	if (functionName == "TanH")
		mOutputValue = TanH(sum);
	if (functionName == "Sigmoid")
		mOutputValue = Sigmoid(sum);
}

void Neuron::CalculateOutputGradients(const float targettValue, const std::string functionName)
{
	float error = targettValue - mOutputValue;
	if (functionName == "TanH")
		mGradient = error * TanHDerivate(mOutputValue);
	if (functionName == "Sigmoid")
		mGradient = error * SigmoidDerivative(mOutputValue);
}

void Neuron::CalculateHiddenGradients(const Layer& nextLayer, const std::string functionName)
{
	float sumDOW = 0.0f;

	for (size_t neuron = 0; neuron + 1 < nextLayer.size(); ++neuron)
		sumDOW += mOutPutWeights[neuron].weight * nextLayer[neuron].mGradient;

	if (functionName == "TanH")
		mGradient = sumDOW * TanHDerivate(mOutputValue);
	if (functionName == "Sigmoid")
		mGradient = sumDOW * SigmoidDerivative(mOutputValue);
}

void Neuron::UpdateInputWeights(Layer& previousLayer)
{
	const float learningRate = 0.15f;

	for (size_t neuron = 0; neuron < previousLayer.size(); ++neuron)
	{
		auto& n = previousLayer[neuron];
		float deltaWeight = learningRate * n.GetOutputValue() * mGradient;
		n.mOutPutWeights[mMyIndex].weight += deltaWeight;
	}
}

NeuralNetwork::NeuralNetwork(const std::vector<size_t>& topology, std::string activationFunction)
{
	const size_t numLayers = topology.size();
	ASSERT(numLayers >= 2, "NeuralNetwork - Invalid Topology, must have atleast 2 layers");

	mFunctionName = activationFunction;
	ASSERT(!mFunctionName.empty(), "NeuralNetwork - Invalid Function type, must have atleast 1 Activation Function");

	mLayers.resize(numLayers);
	for (size_t layer = 0; layer < numLayers; ++layer)
	{
		// Get the number of outputs by looking at the neruon count in the next layer.
		// However, the last layer (i.e. the output layer) has no next layer.
		const size_t numOutputs = (layer + 1 == numLayers) ? 0 : topology[layer + 1];
		const size_t numNeurons = topology[layer];

		for (size_t neuron = 0; neuron <= numNeurons; ++neuron)
		{
			// Every Neuron in this layer has (numOutputs) outputs and (neuron) number
			// For each layer we want 1 extra neuron // 0 - Neuron1
			// as the bias neuron 					 // 1 - Neuron2
			//(hence <= instead of <)				 // |
													 // V
			mLayers[layer].emplace_back(numOutputs, neuron);
		}

		// Force the bias neuron's weight to be always one.
		mLayers[layer].back().SetOutputValue(1.0f);
	}
}

void NeuralNetwork::FeedForward(const std::vector<float>& inputValues)
{
	ASSERT(inputValues.size() == mLayers.front().size() - 1, "NeuralNetwork -- Invalid input count");

	// Assign the input values to the input layer neurons
	for (size_t i = 0; i < inputValues.size(); ++i)
		mLayers.front()[i].SetOutputValue(inputValues[i]);

	// Forward Propagate
	for (size_t layer = 0; layer + 1 < mLayers.size(); ++layer)
	{
		auto& currentLayer = mLayers[layer];
		auto& nextLayer = mLayers[layer + 1];

		for (size_t neuron = 0; neuron + 1 < nextLayer.size(); ++neuron)
			nextLayer[neuron].FeedForward(currentLayer, mFunctionName);
	}
}

void NeuralNetwork::BackPropogate(const std::vector<float>& targetValues)
{
	ASSERT(targetValues.size() + 1 == mLayers.back().size(), "NeuralNetwork -- Invalid output count");

	Layer& outputLayer = mLayers.back();

	// Calculate output layer gradient
	for (size_t neuron = 0; neuron + 1 < outputLayer.size(); ++neuron)
		outputLayer[neuron].CalculateOutputGradients(targetValues[neuron], mFunctionName);

	// calculate gradients on hidden layers
	for (size_t layer = mLayers.size() - 2; layer > 0; --layer)
	{
		Layer& hiddenLayer = mLayers[layer];
		Layer& nextLayer = mLayers[layer + 1];

		for (size_t neuron = 0; neuron < hiddenLayer.size(); ++neuron)
			hiddenLayer[neuron].CalculateHiddenGradients(nextLayer, mFunctionName);
	}

	// Update Connection Weights
	for (size_t layer = mLayers.size() - 1; layer > 0; --layer)
	{
		Layer& currentLayer = mLayers[layer];
		Layer& previousLayer = mLayers[layer - 1];
		for (size_t neuron = 0; neuron + 1 < currentLayer.size(); ++neuron)
			currentLayer[neuron].UpdateInputWeights(previousLayer);
	}

}

std::vector<float> NeuralNetwork::GetResults() const
{
	std::vector<float> results;
	const Layer& outputLayer = mLayers.back();
	for (size_t neuron = 0; neuron + 1 < outputLayer.size(); ++neuron)
		results.push_back(outputLayer[neuron].GetOutputValue());

	return results;
}
