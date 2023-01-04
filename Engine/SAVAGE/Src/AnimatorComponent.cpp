#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
MEMPOOL_DEFINE(AnimatorComponent, 1000)

void AnimatorComponent::Initialize()
{
	mModelComponent = GetOwner().GetComponent<ModelComponent>();
	auto model = ModelManager::Get()->GetModel(mModelComponent->GetModelId());
	mAnimator.Initialize(model);
}

void AnimatorComponent::Update(float deltaTime)
{
	mAnimator.Update(deltaTime);
}
