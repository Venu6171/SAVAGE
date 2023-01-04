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
	SAVAGE::ML::GeneticAlgorithm mGeneticAlgorithm;
	AppLog mAppLog;

	bool mInitialized = false;
	bool mStop = false;
	bool mLast = false;

	ImVec2 mStartPos;
	ImVec2 mNextPos;
	ImVec2 mEndPos;
};