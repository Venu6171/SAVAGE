#include "Precompiled.h"
#include "ModelComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "RenderService.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
MEMPOOL_DEFINE(ModelComponent, 1000)

void ModelComponent::Initialize()
{
	auto renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Register(this);
	
	mModelId = ModelManager::Get()->LoadModel(mFileName, mAnimFileName);
}

void ModelComponent::Terminate()
{
	auto renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Unregister(this);
}
