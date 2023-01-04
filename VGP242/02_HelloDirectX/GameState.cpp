#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::HotPink);
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	auto graphicsSystem = GraphicsSystem::Get();

	if (inputSystem->IsKeyPressed(SAVAGE::Input::KeyCode::ONE))
		graphicsSystem->SetClearColor(SAVAGE::Graphics::Colors::LightGreen);
	if (inputSystem->IsKeyPressed(SAVAGE::Input::KeyCode::TWO))
		graphicsSystem->SetClearColor(SAVAGE::Graphics::Colors::LightPink);
	if (inputSystem->IsKeyPressed(SAVAGE::Input::KeyCode::THREE))
		graphicsSystem->SetClearColor(SAVAGE::Graphics::Colors::LightBlue);
}