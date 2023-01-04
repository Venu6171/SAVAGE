#include "Precompiled.h"
#include "GameWorld.h"
#include "GameObjectIO.h"

using namespace SAVAGE;

void GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld - Already Initialized.");
	for (auto& service : mServices)
		service->Initialize();

	mGameObjects.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);

	mInitialized = true;
}

void GameWorld::Terminate()
{
	ASSERT(!mUpdating, "GameWorld - Cannot terminate world during update.");

	if (!mInitialized)
		return;

	// Destroy all the remaining game objects
	for (auto gameObject : mUpdateList)
		DestroyGameObject(gameObject->GetHandle());

	ProcessDestroyList();
	ASSERT(mUpdateList.empty(), "GameWorld -  Failed to clean up game objects.");

	for (auto& service : mServices)
		service->Terminate();

	mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
	ASSERT(!mUpdating, "GameWorld - Already updating the world.");

	// This defers game object destruction
	mUpdating = true;

	for (auto& service : mServices)
		service->Update(deltaTime);

	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (IsValid(gameObject->GetHandle()))
			gameObject->Update(deltaTime);
	}

	// Allow game object destruction
	mUpdating = false;

	// Now we can safely destory objects
	ProcessDestroyList();
}

void GameWorld::Render()
{
	for (auto& service : mServices)
		service->Render();
}

void GameWorld::DebugUI()
{
	for (auto& service : mServices)
		service->DebugUI();
}

GameObject* GameWorld::CreateGameObject(const std::filesystem::path& templateFileName, std::string name)
{
	ASSERT(mInitialized, "GameWorld - World must be initialized first to create game objects.");
	GameObjectHandle handle;
	if (mFreeSlots.empty())
	{
		return nullptr;
	}

	FILE* file = nullptr;
	auto err = fopen_s(&file, templateFileName.u8string().c_str(), "r");
	if (err != 0 || file == nullptr)
	{
		LOG("Error: failed to open template file %s.", templateFileName.u8string().c_str());
		return nullptr;
	}

	uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	auto& slot = mGameObjects[freeSlot];
	auto& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	GameObjectIO::Read(file, *newObject);
	fclose(file);

	//Initialize handle
	handle.mIndex = freeSlot;
	handle.mGeneration = slot.generation;

	//Initialize GameObject
	newObject->mWorld = this;
	newObject->mHandle = handle;
	newObject->mName = name;
	newObject->Initialize();

	// Add GameObject to update list
	mUpdateList.push_back(newObject.get());

	return newObject.get();
}

GameObject* SAVAGE::GameWorld::FindGameObject(const std::string& name)
{
	auto iter = std::find_if(mUpdateList.begin(), mUpdateList.end(), [&](auto gameObject)
	{
		return gameObject->GetName() == name;
	});
	return iter == mUpdateList.end() ? nullptr : *iter;
}

GameObject* SAVAGE::GameWorld::FindGameObjectWithTag(const std::string& tag)
{
	auto iter = std::find_if(mUpdateList.begin(), mUpdateList.end(), [&](auto gameObject)
	{
		return gameObject->GetTag() == tag;
	});
	return iter == mUpdateList.end() ? nullptr : *iter;
}

std::vector<GameObject*> SAVAGE::GameWorld::FindGameObjectsWithTag(const std::string& tag)
{
	std::vector<GameObject*> objects;

	for (auto& gameObject : mUpdateList)
	{
		if (gameObject->GetTag() == tag)
			objects.push_back(gameObject);
	}

	return objects;
}

GameObject* GameWorld::GetGameObject(GameObjectHandle handle)
{
	if (IsValid(handle))
	{
		auto& slot = mGameObjects[handle.mIndex];
		if (slot.generation == handle.mGeneration)
		{
			return slot.gameObject.get();
		}
	}
	return nullptr;
}

void GameWorld::DestroyGameObject(GameObjectHandle handle)
{
	ASSERT(mInitialized, "GameWorld - World must be initialized first before destroying game objects.");

	if (!IsValid(handle))
		return;

	auto& slot = mGameObjects[handle.mIndex];
	if (slot.generation != handle.mGeneration)
		return;

	slot.generation++;// This invalidates all existing handles to the slot
	mToBeDestroyed.push_back(handle.mIndex);
}

bool GameWorld::IsValid(GameObjectHandle handle) const
{
	return
		handle.mIndex >= 0
		&& handle.mIndex < mGameObjects.size()
		&& mGameObjects[handle.mIndex].generation == handle.mGeneration;
}

void SAVAGE::GameWorld::ProcessDestroyList()
{
	ASSERT(!mUpdating, "GameWorld - Cannot destory game objects during update.");

	for (auto index : mToBeDestroyed)
	{
		auto& slot = mGameObjects[index];

		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld -  Game object is still valid");

		// Remove GameObject from update list
		mUpdateList.erase(std::remove(mUpdateList.begin(), mUpdateList.end(), gameObject)); // erase remove idiom

		// Terminate game object
		gameObject->Terminate();

		// Delete the game object and recycle the slot
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}

	mToBeDestroyed.clear();
}