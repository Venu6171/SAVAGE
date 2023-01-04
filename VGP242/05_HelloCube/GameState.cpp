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
	//
	// TODO:
	// - Create data for vertex buffer and index buffer for a cube
	// 	 i.e.
	//		std::vector<PCEngine::Graphics::VertexPC> vertices;
	//		std::vector<uint32_t> indices;
	//
	// - Initialize MeshBuffer using the new Initialize overload

	mMesh = MeshBuilder::CreateCubePC();

	mVertexBuffer.Initialize(mMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/DoTransform.fx", VertexPC::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTransform.fx");

	mConstantBuffer.Initialize();

	mPosition.x = -1.0f;
	mPosition.z = 5.0f;
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();

	mVertexBuffer.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mRotation.y += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mRotation.y -= deltaTime;
	}

	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mRotation.x += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mRotation.x -= deltaTime;
	}

	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	// Attach buffer to graphics pipeline
	mConstantBuffer.BindVS(0);

	mVertexShader.Bind();
	mPixelShader.Bind();

	Matrix4 matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	TransformData transformData;

	//Set Matrix then draw Cube
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mVertexBuffer.Render();

	//Update Matrix, then draw a second cube
	matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 3.0f, 0.0f, 3.0f });
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mVertexBuffer.Render();

}

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands