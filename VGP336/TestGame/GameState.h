#pragma once

#include "SAVAGE/Inc/SAVAGE.h"
#include <map>

class GameState : public SAVAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render();
	void DebugUI() override;

private:
	void DisplayTriangle(SAVAGE::GameObject* object);

	SAVAGE::GameWorld mGameWorld;
	std::vector<SAVAGE::GameObject*> mObjectHandles;
	SAVAGE::RenderService* mRenderService = nullptr;

	SAVAGE::Math::Vector3 mGameObjectPosition;
	SAVAGE::Math::Vector3 mColliderSetting = { 0.5f, 1.0f, 0.5f };

	char mModelName[256] = { "\0" };
	char mTag[256] = { "\0" };
	char mAddTag[256] = { "\0" };

	std::string mObjectToDestroy;
	std::string mGameObjectName;
	std::string mModelToLoad;
	std::vector<std::pair<std::string, std::string>> mGameObjectValue;

	float mFPS = 0.0f;
	float mTimer = 0.0f;

	float xOffset = 0.175f;
	float yOffset = 1.75f;
	float mMinY_Offset = 1.6f;
	float mMaxY_Offset = 2.15f;
	float mAnimationTimer = 0.0f;

	bool mPlayAnimation = false;
	bool mToggleSkeleton = false;
	bool mShowCollider = false;
	bool mShowTransform = false;
	bool mShowEnemySphere = false;
	bool mDestroyEnemies = false;
	bool mDisplayObejectsWithTag = false;
	bool mShowDestroyButton = false;
	bool mTagAdded = false;

	bool mMax = false;
	bool mMin = false;

	float mEnemyWalkSpeed = 0.0f;
	float mEnemyRunSpeed = 0.0f;

	int spawnId = 0;
};