#pragma once

namespace SAVAGE::ML
{
	// Each Neuron has it's own layer
	class Neuron;
	using Layer = std::vector<Neuron>;

	// Connection between each neuron with their own weights
	struct Connection
	{
		float weight = 0.0f;
	};

	// Each layer has number of Neurons and each Neuron has it's own index 
	// and are connected with each other. Each Neuron gives an value multiplied by
	// weight
	// Neuron also called a Perceptron
	class Neuron
	{
	public:
		Neuron(size_t numOutputs, size_t myIndex);
		void SetOutputValue(float value) { mOutputValue = value; }
		float GetOutputValue() const { return mOutputValue; }
		void FeedForward(const Layer& previousLayer, const std::string functionName);
		void CalculateOutputGradients(float targettValue, const std::string functionName);
		void CalculateHiddenGradients(const Layer& nextLayer, const std::string functionName);
		void UpdateInputWeights(Layer& previousLayer);

	private:
		std::vector<Connection> mOutPutWeights;
		size_t mMyIndex = 0;
		float mOutputValue = 0.0f;
		float mGradient = 0.0f;
	};

	// Topology: Structure of Artificial Neural Network which shows number per each layer
	// Neural Network is basically an array of layers based on the Topology
	class NeuralNetwork
	{
	public:
		// Default Activation Function used is TanH.
		// Currently you can choose from TanH and Sigmoid functions.
		// Simply type the name of the function you want to use.
		NeuralNetwork(const std::vector<size_t>& topology, std::string activationFunction = "TanH");
		void FeedForward(const std::vector<float>& inputValues);
		void BackPropogate(const std::vector<float>& targetValues);
		std::vector<float> GetResults() const;
	private:
		std::vector<Layer> mLayers;
		std::string mFunctionName;
	};
}