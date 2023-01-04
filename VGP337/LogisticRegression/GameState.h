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
	void TrainModel();
	void ResetValues();
	void ShowGraph();

	AppLog mAppLog;
	SAVAGE::ML::Dataset<2> mDataset;
	SAVAGE::ML::LogisticRegression mLogReg;

	int mLogCount = 0;

	bool mPressed = false;
	bool mGenerateData = false;
};