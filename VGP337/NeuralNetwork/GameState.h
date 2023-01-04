#pragma once
#include <SAVAGE/Inc/SAVAGE.h>
#include <ImGui\Inc\AppLog.h>

class GameState : public SAVAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

private:
	struct Dataset
	{
		std::vector<std::vector<float>> inputValues;
		std::vector<std::vector<float>> outputValues;
	};

	Dataset mDataset;
	std::unique_ptr<SAVAGE::ML::NeuralNetwork> neuralNetwork;
	std::vector<size_t> topology;
	int mCurrentEntry = 0;
};