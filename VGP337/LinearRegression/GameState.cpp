#include "GameState.h"
#include <chrono>
#include <ImGui\Inc\ImPlot.h>

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;
using namespace SAVAGE::ML;

namespace
{
	float b0 = 0.0f;
	float b1 = 0.0f;
	float minX = 0.0f;
	float maxX = 0.0f;
	float noise = 0.0f;
	float learningRate = 0.0f;
	size_t samples = 0;
	int numCycles = 0;
}

void GameState::Initialize()
{
	samples = 100;
	b0 = 200.0f;
	b1 = -1.2f;
	minX = 0.0f;
	maxX = 100.0f;
	noise = 1.0f;
	learningRate = 0.0005f;
	numCycles = 100;

	//mDataset = Datasets::MakeLinear(mSamples, mB0, mB1, mMinX, mMaxX, mNoise);
	//mLinReg.learningRate = mLearningRate;

	mAppLog.AddLog("Click CreateData to genarate data and scatter the values on graph.\n");
	mAppLog.AddLog("Click Train to train the model.\n");
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		SAVAGE::MainApp().Quit();

	if (inputSystem->IsKeyPressed(KeyCode::SPACE))
		mAppLog.AddLog("Bam!\n");

	if (mGenerateData)
	{
		GenerateData();
	}

	if (mPressed)
	{
		TrainModel();
	}

}

void GameState::DebugUI()
{
	//ImGui::Begin("Debug Values", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	PlotGraph();

	ImGui::InputInt("Number of Samples", (int*)&samples, 10);
	ImGui::InputInt("Cycles", &numCycles, 500);
	ImGui::DragFloat("b0", &b0, 0.1f, -500.0f, 500.0f);
	ImGui::DragFloat("b1", &b1, 0.1f, -500.0f, 500.0f);
	ImGui::DragFloat("minX", &minX, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("maxX", &maxX, 1.0f, 100.0f, 500.0f);
	ImGui::DragFloat("Noise", &noise, 0.5f, 1.0f, 50.0f);
	ImGui::DragFloat("Learning Rate", &learningRate, 0.000001f, 0.000001f, 1.0f, "%.7f");

	if (ImGui::Button("Generate Data"))
	{
		mGenerateData = true;
		//GenerateData();
	}

	if (ImGui::Button("Train"))
	{
		mPressed = true;
		//TrainModel();
	}

	if (ImGui::Button("Reset"))
		ResetValues();

	//ImGui::End();
	mAppLog.Draw("Console");
}

void GameState::PlotGraph()
{
	if (ImPlot::BeginPlot("Linear Regression", "x", "y"))
	{
		float startX = minX;
		float endX = maxX;
		float startY = mLinReg.Predict(startX);
		float endY = mLinReg.Predict(endX);
		std::vector<ImVec2> line;

		line.push_back({ startX, startY });
		line.push_back({ endX, endY });

		for (size_t i = 0; i < mDataset.x.size(); ++i)
			ImPlot::PlotScatter("DataSet", mDataset.x[0].data(), mDataset.y.data(), mDataset.x.size());

		ImPlot::PlotLine("Model", line.data(), line.size());
		ImPlot::EndPlot();
	}
}

void GameState::GenerateData()
{
	mDataset = Datasets::MakeLinear(samples, b0, b1, minX, maxX, noise);
	mLinReg.learningRate = learningRate;
	mGenerateData = false;
}

void GameState::TrainModel()
{
	for (int i = 0; i < numCycles; ++i)
	{
		float error = mLinReg.Fit(mDataset);
		mAppLog.AddLog("b0 = %f, b1 = %f, error = %f\n", mLinReg.b0, mLinReg.b1, error);

		if (fabs(error) < 1.0f)
		{
			mPressed = false;
			mAppLog.AddLog("Final values are: B0=%f B1=%f error=%f\n", mLinReg.b0, mLinReg.b1, mLinReg.Fit(mDataset));
			break;
		}
	}
}

void GameState::ResetValues()
{
	mAppLog.Clear();
	mPressed = false;
	mGenerateData = false;
	//mSamples = 0;
	//mB0 = 1.0f;
	//mB1 = 0.1f;
	//mMinX = 0.0f;
	//mMaxX = 10.0f;
	//mNoise = 0.1f;
	//mLearningRate = 0.00001f;
	//mNumCycles = 100;

	mLinReg.b0 = 0.0f;
	mLinReg.b1 = 0.0f;
}