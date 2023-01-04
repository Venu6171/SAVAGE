#include "GameState.h"
#include "SAVAGE\Inc\Component.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize(1000);
	mRenderService = mGameWorld.GetService<RenderService>();
	mObjectHandles.push_back(mGameWorld.CreateGameObject("../../Assets/Templates/Player.json", "Viren"));
	mObjectHandles.push_back(mGameWorld.CreateGameObject("../../Assets/Templates/EnemyMutant.json", "Peter"));
	mObjectHandles.push_back(mGameWorld.CreateGameObject("../../Assets/Templates/EnemyHulk.json", "Manhar"));
	mObjectHandles.push_back(mGameWorld.CreateGameObject("../../Assets/Templates/EnemyBrute.json", "Devesh"));

	mGameObjectValue.push_back(std::make_pair("Brute", "../../Assets/Templates/EnemyBrute.json"));
	mGameObjectValue.push_back(std::make_pair("Mutant", "../../Assets/Templates/EnemyMutant.json"));
	mGameObjectValue.push_back(std::make_pair("PumpkinHulk", "../../Assets/Templates/EnemyHulk.json"));
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

	if (mTagAdded)
		mTimer += deltaTime;

	if (mTimer > 3.0f)
	{
		mTimer = 0.0f;
		mTagAdded = false;
	}

	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::Checkbox("ToggleSkeleton", &mToggleSkeleton))
	{
		mRenderService->SetToggleSkeleton(mToggleSkeleton);
	}

	if (ImGui::Checkbox("ShowTransform", &mShowTransform))
	{
		for (auto& gameObjects : mObjectHandles)
		{
			auto transformComponent = gameObjects->GetComponent<TransformComponent>();
			if (transformComponent)
				transformComponent->SetShowTransform(mShowTransform);
		}
	}

	if (ImGui::Checkbox("ShowEnemySphere", &mShowEnemySphere))
	{
		for (auto& gameObjects : mObjectHandles)
		{
			auto enemyComponent = gameObjects->GetComponent<EnemyComponent>();
			if (enemyComponent)
				enemyComponent->SetShowSphere(mShowEnemySphere);
		}
	}

	if (ImGui::CollapsingHeader("ColliderComponent"))
	{
		if (ImGui::Checkbox("ShowCollider", &mShowCollider))
		{
			for (auto& gameObjects : mObjectHandles)
			{
				auto colliderComponent = gameObjects->GetComponent<ColliderComponent>();
				if (colliderComponent)
					colliderComponent->SetShowCollider(mShowCollider);
			}
		}
		ImGui::Text("ColliderSize");
		ImGui::SameLine();
		if (ImGui::DragFloat3("#", &mColliderSetting.x, 0.1f, 0.1f, 2.0f))
		{
			for (auto& gameObjects : mObjectHandles)
			{
				auto colliderComponent = gameObjects->GetComponent<ColliderComponent>();
				if (colliderComponent)
					colliderComponent->SetExtend(mColliderSetting);
			}
		}
	}
	static std::string currentObjectName;
	if (ImGui::CollapsingHeader("CreateGameObject"))
	{
		ImGui::Text("Choose Game Object");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##", currentObjectName.c_str()))
		{
			for (auto& object : mGameObjectValue)
			{
				bool isSelected = (currentObjectName == object.first);
				if (ImGui::Selectable(object.first.c_str(), isSelected))
					currentObjectName = object.first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				if (currentObjectName == object.first)
					mModelToLoad = object.second;
			}
			ImGui::EndCombo();
		}

		if (!currentObjectName.empty())
		{
			ImGui::Text("Model Name");
			ImGui::SameLine();
			if (ImGui::InputText("#-#", mModelName, std::size(mModelName)))
				mGameObjectName = mModelName;
			if (ImGui::DragFloat("SetWalkSpeed", &mEnemyWalkSpeed))

			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::DragFloat3("###", &mGameObjectPosition.x, 0.1f, -50.0f, 50.0f);
			if (ImGui::Button("RandomPosition"))
				mGameObjectPosition = SAVAGE::Math::RandomVector3({ -20.0f, 0.0f, -20.0f }, { 20.0f, 0.0f, 20.0f });
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				memset(mModelName, 0, std::size(mModelName));
				if (mGameObjectName.empty())
				{
					mGameObjectName = "GameObject" + spawnId;
					spawnId++;
				}

				auto gameObject = mGameWorld.CreateGameObject(mModelToLoad, mGameObjectName);
				gameObject->GetComponent<TransformComponent>()->SetPosition(mGameObjectPosition);
				mGameObjectPosition.x += 2.0f;
				mObjectHandles.push_back(gameObject);
			}
		}
	}
	else
		currentObjectName = "";

	static std::string objectToDestroy;
	std::vector<std::string> namesToDestroy;
	namesToDestroy.push_back("None");
	for (auto& names : mObjectHandles)
		namesToDestroy.push_back(names->GetName());

	if (ImGui::CollapsingHeader("DestroyGameObject"))
	{
		ImGui::Text("Choose Game Object");
		ImGui::SameLine();
		if (ImGui::BeginCombo("--#", objectToDestroy.c_str()))
		{
			for (auto& object : namesToDestroy)
			{
				bool is_selected = (objectToDestroy == object);
				if (ImGui::Selectable(object.c_str(), is_selected))
					objectToDestroy = object;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
				if (objectToDestroy == object)
					mObjectToDestroy = object;
			}
			ImGui::EndCombo();
		}
		if (!objectToDestroy.empty() && objectToDestroy != "None")
		{
			DisplayTriangle(mGameWorld.FindGameObject(objectToDestroy));

			if (objectToDestroy != "Viren")
			{
				if (ImGui::Button("DestroyObject"))
				{
					auto gameObject = mGameWorld.FindGameObject(mObjectToDestroy);
					mGameWorld.DestroyGameObject(gameObject->GetHandle());
					auto destroyedObject = std::find(mObjectHandles.begin(), mObjectHandles.end(), gameObject);
					mObjectHandles.erase(destroyedObject);
					objectToDestroy = "";
				}
			}
		}
	}

	static std::string currentObject;
	if (ImGui::CollapsingHeader("Add Tag to GameObjects"))
	{
		ImGui::Text("Select GameObject");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##--##", currentObject.c_str()))
		{
			for (auto& object : namesToDestroy)
			{
				bool is_selected = (currentObject == object);
				if (ImGui::Selectable(object.c_str(), is_selected))
					currentObject = object;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (!currentObject.empty() && currentObject != "None")
		{
			ImGui::Text("Add Tag:");
			ImGui::SameLine();
			ImGui::InputText("#####", mAddTag, std::size(mAddTag));
			if (ImGui::Button("Add"))
			{
				for (auto& gameObject : mObjectHandles)
				{
					if (currentObject == gameObject->GetName())
					{
						gameObject->AddTag(mAddTag);
						mTagAdded = true;
					}
				}
				currentObject = "";
				memset(mAddTag, 0, std::size(mAddTag));
			}

			if (mTagAdded)
				ImGui::Text("Tag Added.");
		}
	}
	else
		currentObject = "";

	ImGui::Text("Find Objects with Tag:");
	ImGui::SameLine();
	ImGui::InputText("", mTag, std::size(mTag));

	if (strcmp(mTag, "") == 0)
		mDisplayObejectsWithTag = false;

	if (ImGui::Button("Find"))
	{
		auto objects = mGameWorld.FindGameObjectsWithTag(mTag);
		if (!objects.empty())
			mDisplayObejectsWithTag = true;
	}

	ImGui::SameLine();
	if (ImGui::Button("Clear"))
	{
		mDisplayObejectsWithTag = false;
		mShowDestroyButton = false;
		memset(mTag, 0, std::size(mTag));
	}

	if (mDisplayObejectsWithTag)
	{
		auto gameObjects = mGameWorld.FindGameObjectsWithTag(mTag);
		for (auto& object : gameObjects)
		{
			DisplayTriangle(object);
			auto modelName = object->GetComponent<ModelComponent>()->GetModelName();
			auto modelId = object->GetComponent<ModelComponent>()->GetModelId();
			ImGui::Text("GameObject- ModelName: '%s' ObjectName: '%s', Tag: '%s', ID: '%d'", modelName.c_str(), object->GetName().c_str(), object->GetTag().c_str(), modelId);
		}
		mShowDestroyButton = strcmp(mTag, "Player") != 0;
	}
	else
	{
		for (auto& object : mObjectHandles)
		{
			auto modelName = object->GetComponent<ModelComponent>()->GetModelName();
			auto modelId = object->GetComponent<ModelComponent>()->GetModelId();
			ImGui::Text("GameObject- ModelName: '%s', ObjectName: '%s', Tag: '%s', ID: '%d'", modelName.c_str(), object->GetName().c_str(), object->GetTag().c_str(), modelId);
		}
	}

	if (mShowDestroyButton)
		if (ImGui::Button("DestroyObjects"))
		{
			auto gameObjects = mGameWorld.FindGameObjectsWithTag(mTag);
			for (auto& object : gameObjects)
			{
				mGameWorld.DestroyGameObject(object->GetHandle());
				auto destroyedObject = std::find(mObjectHandles.begin(), mObjectHandles.end(), object);
				mObjectHandles.erase(destroyedObject);
			}
		}
	ImGui::End();

	for (auto& gameObject : mObjectHandles)
		gameObject->DebugUI();

	mGameWorld.DebugUI();
}

void GameState::DisplayTriangle(SAVAGE::GameObject* object)
{
	/*if (mMin)
	{
		yOffset += 0.01f;
		mMinY_Offset += 0.01f;
	}
	else if (mMax)
	{
		yOffset -= 0.01f;
		mMinY_Offset -= 0.01f;
	}

	if (yOffset >= mMaxY_Offset && mMinY_Offset >= 2.0f)
	{
		mMax = true;
		mMin = false;
	}
	else if (yOffset <= 1.85f && mMinY_Offset <= 1.7f)
	{
		mMin = true;
		mMax = false;
	}

	auto& position = object->GetComponent<TransformComponent>()->GetPosition();
	SimpleDraw::AddTriangle(
		{ position.x + xOffset, position.y + yOffset, position.z },
		{ position.x - xOffset, position.y + yOffset, position.z },
		{ position.x, position.y + mMinY_Offset, position.z }, Colors::Red);*/
}