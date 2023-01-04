#include "Precompiled.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "ModelComponent.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Math;
MEMPOOL_DEFINE(HealthComponent, 1000)

void HealthComponent::Initialize()
{
	mModelComponent = GetOwner().GetComponent<ModelComponent>();
	
	GetHealth();
}

void HealthComponent::Update(float deltaTime)
{
}