#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	// NDC - Normalized Device Coordinate
	//
	//          +-----------------+
	//         /                 /|
	//   1.0  +-----------------+ |
	//	      |        ^        | |
	//	      |        |        | |
	//	      | <------+------> | |
	//	      |        |        | | 1.0
	//	      |        v        |/
	//	-1.0  +-----------------+ 0.0
	//      -1.0               1.0



	//-------------------------------------------
	//*****************Heart*********************
	//-------------------------------------------

	//Middle
	mHeartVertices.push_back({ Vector3{ 0.0f, 0.1f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.1f, -0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.1f, -0.09f, 0.0f }, Colors::Red });

	//***************Right*****************

	//Left
	mHeartVertices.push_back({ Vector3{ 0.105f, 0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.105f, -0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.005f, 0.1f, 0.0f }, Colors::Red });

	//Right
	mHeartVertices.push_back({ Vector3{ 0.110f, 0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.210f, 0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.110f, -0.09f, 0.0f }, Colors::Red });

	//Top Left
	mHeartVertices.push_back({ Vector3{ 0.105f, 0.295f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.105f, 0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.005f, 0.105f, 0.0f }, Colors::Red });

	//Top Right
	mHeartVertices.push_back({ Vector3{ 0.110f, 0.295f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.210f, 0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.110f, 0.095f, 0.0f }, Colors::Red });

	// Extreme Corner Right
	mHeartVertices.push_back({ Vector3{ 0.215f, 0.090f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.205f, -0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.115f, -0.09f, 0.0f }, Colors::Red });

	//*****************Left********************

	//Right
	mHeartVertices.push_back({ Vector3{ -0.105f, 0.1f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.005f, 0.1f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.105f, -0.09f, 0.0f }, Colors::Red });

	//Left
	mHeartVertices.push_back({ Vector3{ -0.110f, 0.1f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.110f, -0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.210f, 0.1f, 0.0f }, Colors::Red });

	//Top Right
	mHeartVertices.push_back({ Vector3{ -0.105f, 0.295f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.005f, 0.105f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.105, 0.105f, 0.0f }, Colors::Red });

	//Top Left
	mHeartVertices.push_back({ Vector3{ -0.110f, 0.295f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.110f, 0.105f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.210f, 0.105f, 0.0f }, Colors::Red });

	// Extreme Corner Left
	mHeartVertices.push_back({ Vector3{ -0.215f, 0.1f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.115f, -0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.215f, -0.09f, 0.0f }, Colors::Red });

	//***********Bottom*************

	//Middle
	mHeartVertices.push_back({ Vector3{ 0.0f, -0.09f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.1f, -0.29f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.1f, -0.29f, 0.0f }, Colors::Red });

	//Right
	mHeartVertices.push_back({ Vector3{ 0.105f, -0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.105f, -0.290f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.005f, -0.095f, 0.0f }, Colors::Red });

	//Extreme Right
	mHeartVertices.push_back({ Vector3{ 0.110f, -0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.205f, -0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.110f, -0.290f, 0.0f }, Colors::Red });

	//Left
	mHeartVertices.push_back({ Vector3{ -0.105f, -0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.005f, -0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.105f, -0.290f, 0.0f }, Colors::Red });

	//Extreme Left
	mHeartVertices.push_back({ Vector3{ -0.110f, -0.095f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.110f, -0.290f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.215f, -0.095f, 0.0f }, Colors::Red });

	//Bottom Right
	mHeartVertices.push_back({ Vector3{ 0.0f, -0.295f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.105f, -0.295f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ 0.0f, -0.5f, 0.0f }, Colors::Red });

	//Bottom Left
	mHeartVertices.push_back({ Vector3{ -0.005f, -0.295f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.005f, -0.5f, 0.0f }, Colors::Red });
	mHeartVertices.push_back({ Vector3{ -0.105f, -0.295f, 0.0f }, Colors::Red });


	// ---------------------------------------------
	// ****************TriForce*********************
	// ---------------------------------------------

	//Top Triangle
	mTriforceVertices.push_back({ Vector3{ 0.0f, 0.8f, 0.0f }, Colors::Red });
	mTriforceVertices.push_back({ Vector3{ 0.2f, 0.1f, 0.0f }, Colors::Green });
	mTriforceVertices.push_back({ Vector3{ -0.2f, 0.1f, 0.0f }, Colors::Blue });

	//Right Triangle
	mTriforceVertices.push_back({ Vector3{ 0.2f, 0.1f, 0.0f }, Colors::Red });
	mTriforceVertices.push_back({ Vector3{ 0.4f, -0.6f, 0.0f }, Colors::Green });
	mTriforceVertices.push_back({ Vector3{ 0.0f, -0.6f, 0.0f }, Colors::Blue });

	//Left Triangle
	mTriforceVertices.push_back({ Vector3{ -0.2f, 0.1f, 0.0f }, Colors::Red });
	mTriforceVertices.push_back({ Vector3{ 0.0f, -0.6f, 0.0f  }, Colors::Green });
	mTriforceVertices.push_back({ Vector3{ -0.4f, -0.6f, 0.0f }, Colors::Blue });

	mSwordVertices.push_back({ Vector3{ 0.0f, 0.5f, 0.0f }, Colors::Blue });
	mSwordVertices.push_back({ Vector3{ 0.5f, -0.5f, 0.0f }, Colors::Blue });
	mSwordVertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f }, Colors::Blue });

	mVertexBuffer[0].Initialize(mHeartVertices.data(), sizeof(VertexPC), (uint32_t)mHeartVertices.size());
	mVertexBuffer[1].Initialize(mTriforceVertices.data(), sizeof(VertexPC), (uint32_t)mTriforceVertices.size());
	mVertexBuffer[2].Initialize(mSwordVertices.data(), sizeof(VertexPC), (uint32_t)mSwordVertices.size());

	// Default to the first buffer
	mCurrentVertexBuffer = &mVertexBuffer[0];

	mVertexShader.Initialize(L"../../Assets/Shaders/DoSomething.fx", VertexPC::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");
}

void GameState::Terminate()
{
	for (int i = 0; i < std::size(mVertexBuffer); i++)
		mVertexBuffer[i].Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ONE))
	{
		mCurrentVertexBuffer = &mVertexBuffer[0];
	}

	if (inputSystem->IsKeyPressed(KeyCode::TWO))
	{
		mCurrentVertexBuffer = &mVertexBuffer[1];
	}

	if (inputSystem->IsKeyPressed(KeyCode::THREE))
	{
		mCurrentVertexBuffer = &mVertexBuffer[2];
	}
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mCurrentVertexBuffer->Render();
}

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands


// Add 04_HelloShapes project
// - Copy code from 03_HelloTriangle
// - Update to use DoSomething.fx shaders
// - Create multiple meshes and vertex buffers
// - Draw a heart
// - Draw a triforce
// - Draw something with more than 5 triangles
// - Use input check so you can change between them
// Description: Simple shader that does nothing.

// Device Interface - used for VRAM allocation/resource creation
// Context Interface - used to issue draw commands