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
	struct Unit
	{
		ImVec2 position;
		ImVec2 velocity;
		size_t cluster = 0;
	};

	AppLog mAppLog;
	SAVAGE::Graphics::Texture mUnitTexture;
	std::vector<Unit> mUnits;

	bool mShowCluster = false;
};