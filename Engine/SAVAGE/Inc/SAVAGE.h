#pragma once

#include "Common.h"

// App headers
#include "App.h"
#include "AppState.h"

// Component headers
#include "AnimatorComponent.h"
#include "CharacterControllerComponent.h"
#include "Component.h"
#include "ColliderComponent.h"
#include "EnemyComponent.h"
#include "HealthComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

// RenderService header
#include "RenderService.h"

// World headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameObjectIO.h"
#include "GameWorld.h"

namespace SAVAGE
{
	App& MainApp();
}