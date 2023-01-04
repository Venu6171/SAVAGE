#include "Precompiled.h"
#include "App.h"

#include "AppState.h"

using namespace SAVAGE;
using namespace SAVAGE::Core;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void App::ChangeState(const std::string& name)
{
	auto iter = mAppStates.find(name);
	if (iter != mAppStates.end())
	{
		LOG("App -- Changing state: %s", name.c_str());
		mNextState = iter->second.get();
	}
}

void App::Run(const AppConfig& appConfig)
{
	LOG("App -- Running ...");

	LOG("App -- Creating window ...");
	Window window;
	window.Initialize(
		GetModuleHandle(NULL),
		appConfig.appName,
		appConfig.windowWidth,
		appConfig.windowHeight);

	auto handle = window.GetWindowHandle();

	InputSystem::StaticInitialize(handle);
	GraphicsSystem::StaticInitialize(handle, false);
	ModelManager::StaticInitialize();
	SimpleDraw::StaticInitialize(20000);
	DebugUI::StaticInitialize(handle, false, true);

	// Initialize the starting state
	ASSERT(mCurrentState, "App -- No app state found! App must have at least one AppState.");
	mCurrentState->Initialize();

	LOG("App -- Entering main loop ...");

	mRunning = true;
	while (mRunning && window.IsActive())
	{
		window.ProcessMessage();
		if (!window.IsActive())
			continue;

		if (mNextState)
		{
			LOG("App -- Changing app state ...");
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (appConfig.escToQuit && inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			LOG("App -- ESC pressed. Exiting app ...");
			Quit();
			continue;
		}

		auto deltaTime = TimeUtil::GetDeltaTime();
		mCurrentState->Update(deltaTime);

		auto graphicsSystem = GraphicsSystem::Get();
		graphicsSystem->BeginRender();

		mCurrentState->Render();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		graphicsSystem->EndRender();
	}

	LOG("App -- Shutting down ...");

	// Clean up current state
	mCurrentState->Terminate();

	DebugUI::StaticTerminate();
	SimpleDraw::StaticTerminate();
	ModelManager::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	window.Terminate();

	LOG("App -- Exiting ...");
}