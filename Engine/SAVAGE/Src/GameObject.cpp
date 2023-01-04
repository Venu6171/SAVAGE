#include "Precompiled.h"
#include "GameObject.h"
#include "Component.h"

using namespace SAVAGE;

MEMPOOL_DEFINE(GameObject, 1000);

void SAVAGE::GameObject::Initialize()
{
	ASSERT(!mInitialized, "GameObject - Already initialized.");
	for (auto& component : mComponents)
		component->Initialize();

	mInitialized = true;
}

void SAVAGE::GameObject::Terminate()
{
	for (auto& component : mComponents)
		component->Terminate();
}

void SAVAGE::GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
		component->Update(deltaTime);
}

void SAVAGE::GameObject::DebugUI()
{
	for (auto& component : mComponents)
		component->DebugUI();
}