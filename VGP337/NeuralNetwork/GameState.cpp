#include "GameState.h"
#include <fstream>
#include <sstream>

using namespace SAVAGE;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;
using namespace SAVAGE::ML;

void GameState::Initialize()
{
	const std::vector<std::vector<float>> digitsOutputValues =
	{
		 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		 {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
		 {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		 {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
		 {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		 {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
	};

	std::fstream file;
	file.open("optdigits.tra", std::ios::in);

	std::string line;
	while (getline(file, line))
	{
		mDataset.inputValues.push_back({});
		mDataset.outputValues.push_back({});

		auto& input = mDataset.inputValues.back();
		auto& output = mDataset.outputValues.back();
		input.reserve(64);
		output.reserve(10);

		std::istringstream ss(line);
		std::string token;
		while (getline(ss, token, ','))
		{
			if (input.size() < 64)
				input.push_back(std::stof(token) / 16.0f);
			else
			{
				const int digit = std::stoi(token);
				output = digitsOutputValues[digit];
			}
		}
	}

	topology.push_back(64);
	topology.push_back(32);
	topology.push_back(10);
	neuralNetwork = std::make_unique<NeuralNetwork>(topology);
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		SAVAGE::MainApp().Quit();

}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Values", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::End();

	ImVec2 offset{ 0.0f, 20.0f };
	ImGui::SetNextWindowSize(ImVec2{ 128.0f, 128.0f } + offset);
	ImGui::Begin("Target", nullptr, ImGuiWindowFlags_NoResize);
	ImVec2 winPos = ImGui::GetWindowPos();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	for (size_t y = 0; y < 8; ++y)
	{
		for (size_t x = 0; x < 8; ++x)
		{
			int i = x + (y * 8);
			ImVec2 min{ x * 16.0f, y * 16.0f };
			ImVec2 max = min + ImVec2{ 16.0f, 16.0f };
			float intensity = mDataset.inputValues[mCurrentEntry][i];
			ImColor color{ intensity, intensity, intensity };
			drawList->AddRectFilled(min + winPos + offset, max + winPos + offset, color);
		}
	}

	ImGui::End();
}