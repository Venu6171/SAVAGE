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
	float b2 = 0.0f;
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
	b2 = 0.2f;
	minX = 0.0f;
	maxX = 100.0f;
	noise = 1.0f;
	learningRate = 0.0005f;
	numCycles = 100;

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
		mDataset = Datasets::MakeLogistic(samples, b0, b1, b2, minX, maxX, noise);
		mLogReg.learningRate = learningRate;
		mGenerateData = false;
	}

	if (mPressed)
	{
		for (int i = 0; i < numCycles; ++i)
		{
			float error = mLogReg.Fit(mDataset);
			mAppLog.AddLog("b0 = %f, b1 = %f, b2 = %f, error = %f\n", mLogReg.b0, mLogReg.b1, mLogReg.b2, error);

			if (fabs(error) < 1.0f)
			{
				mPressed = false;
				mAppLog.AddLog("Final values are: B0=%f B1=%f B2=%f error=%f\n", mLogReg.b0, mLogReg.b1, mLogReg.b2, error);
				mAppLog.AddLog("Time took: %f delta seconds\n", deltaTime);
				break;
			}
		}
	}

}

void GameState::DebugUI()
{
	//ImGui::Begin("Debug Values", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ShowGraph();

	ImGui::InputInt("Number of Samples", (int*)&samples, 10);
	ImGui::InputInt("Cycles", &numCycles, 500);
	ImGui::DragFloat("b0", &b0, 0.1f, -500.0f, 500.0f);
	ImGui::DragFloat("b1", &b1, 0.1f, -500.0f, 500.0f);
	ImGui::DragFloat("b2", &b2, 0.1f, -500.0f, 500.0f);
	ImGui::DragFloat("minX", &minX, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("maxX", &maxX, 1.0f, 100.0f, 500.0f);
	ImGui::DragFloat("Noise", &noise, 0.5f, 1.0f, 50.0f);
	ImGui::DragFloat("Learning Rate", &learningRate, 0.000001f, 0.000001f, 1.0f, "%.7f");

	if (ImGui::Button("Generate Data"))
		mGenerateData = true;

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

void GameState::TrainModel()
{
	//mLogCount++;
	//
	//if (mLogCount >= 6)
	//{
	//	mAppLog.Clear();
	//	mLogCount = 0;
	//}
	//mDataset = Datasets::MakeLinear(mSamples, mB0, mB1, mMinX, mMaxX, mNoise);
	//mLinReg.learningRate = mLearningRate;

	//auto startTime = std::chrono::steady_clock::now();
	//const int epoch = mNumCycles;
	//for (int e = 0; e < epoch; ++e)
	//{
	//	float error = mLinReg.Fit(mDataset);
	//	mAppLog.AddLog("b0 = %f, b1 = %f, error = %f\n", mLinReg.b0, mLinReg.b1, error);
	//}
	//auto endTime = std::chrono::steady_clock::now();
	//
	//mAppLog.AddLog("Final values are: B0=%f B1=%f error=%f\n", mLinReg.b0, mLinReg.b1, mLinReg.Fit(mDataset));
	//mAppLog.AddLog("Time took: %lld milliseconds\n", std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count());
}

void GameState::ResetValues()
{
	mAppLog.Clear();
	mPressed = false;
	//mSamples = 0;
	//mB0 = 1.0f;
	//mB1 = 0.1f;
	//mMinX = 0.0f;
	//mMaxX = 10.0f;
	//mNoise = 0.1f;
	//mLearningRate = 0.00001f;
	//mNumCycles = 100;

	mLogReg.b0 = 0.0f;
	mLogReg.b1 = 0.0f;
}

void GameState::ShowGraph()
{
	if (ImPlot::BeginPlot("Logistic Regression", "x0", "x1"))
	{
		std::vector<ImVec2> modelLine;
		int count = static_cast<int>(mDataset.x.size());
		modelLine.push_back({ minX, b0 / -b2 });
		modelLine.push_back({ maxX, (b0 + (b1 * 10)) / -b2 });
		ImPlot::PlotLine("Model", modelLine.data(), (int)modelLine.size());

		std::vector<float> ax, ay, bx, by;
		for (int i = 0; i < count; ++i)
		{
			float p = b0 + b1 * mDataset.x[i][0] + b2 * mDataset.x[i][1];
			if (p >= 0)
			{
				ax.push_back(mDataset.x[i][0]);
				ay.push_back(mDataset.x[i][1]);
			}
			else
			{
				bx.push_back(mDataset.x[i][0]);
				by.push_back(mDataset.x[i][1]);
			}
		}

		if (!ax.empty())
			ImPlot::PlotScatter("Ones", ax.data(), ay.data(), (int)ax.size());

		if (!bx.empty())
			ImPlot::PlotScatter("Zeros", bx.data(), by.data(), (int)bx.size());

		ImPlot::EndPlot();
	}
}
