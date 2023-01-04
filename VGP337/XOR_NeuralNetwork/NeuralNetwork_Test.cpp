#include <iostream>
#include <SAVAGE/Inc/SAVAGE.h>

using namespace SAVAGE::ML;
using namespace SAVAGE::Math;

int main()
{
	std::vector<size_t> topology{ 2, 2, 1 };
	NeuralNetwork neuralNetwork(topology);

	std::vector<std::vector<float>> inputValues(4);
	inputValues[0].push_back(0.0f);
	inputValues[0].push_back(0.0f);

	inputValues[1].push_back(0.0f);
	inputValues[1].push_back(1.0f);

	inputValues[2].push_back(1.0f);
	inputValues[2].push_back(0.0f);

	inputValues[3].push_back(1.0f);
	inputValues[3].push_back(1.0f);

	std::vector<std::vector<float>> targetValues(4);
	targetValues[0].push_back(0.0f);
	targetValues[1].push_back(1.0f);
	targetValues[2].push_back(1.0f);
	targetValues[3].push_back(0.0f);

	std::vector<float> outputValues;
	float error = 0.0f;
	int randomCount = 0;
	size_t iter = 0;

	std::cout << "---- Choose DataSet----\n";
	std::cout << "('X' for XOR, 'd' for digits)\n";

	char dataSet{};
	std::cin >> dataSet;
	std::cout << std::endl;
	if (dataSet == 'x' || dataSet == 'X')
	{
		std::cout << "Enter number of iterations to train the data: ";
		std::cin >> iter;
		std::cout << std::endl;

		for (size_t i = 0; i < iter; ++i)
		{
			randomCount = RandomInt(0, 3);
			neuralNetwork.FeedForward(inputValues[randomCount]);
			outputValues = neuralNetwork.GetResults();
			error = outputValues[0] - targetValues[randomCount][0];
			neuralNetwork.BackPropogate(targetValues[randomCount]);

			std::cout << "InputValues: " << inputValues[randomCount][0] << ", " << inputValues[randomCount][1] << std::endl;
			std::cout << "Outputs: " << outputValues[0] << std::endl;
			std::cout << "Actual: " << targetValues[randomCount][0] << std::endl;
			std::cout << "Error: " << error << std::endl;
			std::cout << std::endl;
		}

		std::cout << "---Done Training---\n";
		std::cout << "(Press any key to predict, q to quit)\n";
	}

	std::cin >> dataSet;
	while (dataSet != 'q' && getchar() != 'q')
	{
		randomCount = RandomInt(0, 3);
		neuralNetwork.FeedForward(inputValues[randomCount]);
		outputValues = neuralNetwork.GetResults();
		error = outputValues[0] - targetValues[randomCount][0];
		neuralNetwork.BackPropogate(targetValues[randomCount]);

		std::cout << "InputValues: " << inputValues[randomCount][0] << ", " << inputValues[randomCount][1] << std::endl;
		std::cout << "Outputs: " << outputValues[0] << std::endl;
		std::cout << "Actual: " << targetValues[randomCount][0] << std::endl;
		std::cout << "Error: " << error << std::endl;
	}

	return 0;
}