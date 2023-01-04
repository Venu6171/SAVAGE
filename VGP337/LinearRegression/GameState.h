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
	void PlotGraph();
	void GenerateData();
	void TrainModel();
	void ResetValues();

	AppLog mAppLog;
	SAVAGE::ML::Dataset<1> mDataset;
	SAVAGE::ML::LinearRegression mLinReg;

	int mLogCount = 0;

	bool mPressed = false;
	bool mGenerateData = false;
};