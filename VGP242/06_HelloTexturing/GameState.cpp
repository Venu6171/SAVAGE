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

	//mMesh = MeshBuilder::CreateCubePX();
	//mMesh = MeshBuilder::CreatePlanePX(5, 5, 5);
	mMesh = MeshBuilder::CreateCylinderPX(2, 16, 4.0f, 1.0f);

	mVertexBuffer.Initialize(mMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");

	mSampler[0].Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mTexture[0].Initialize("../../Assets/Images/Hulk.png");

	mSampler[1].Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mTexture[1].Initialize("../../Assets/Images/FallGuy1.png");

	mSampler[2].Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mTexture[2].Initialize("../../Assets/Images/FallGuy2.png");

	//mSampler[3].Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	//mTexture[3].Initialize("../../Assets/Images/FallGuy2.png");

	mConstantBuffer.Initialize();

	mPosition.z = 10.0f;
	mPosition.y = -2.0f;
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	for (int i = 0; i < std::size(mTexture); ++i)
	{
		mTexture[i].Terminate();
	}
	for (int i = 0; i < std::size(mSampler); ++i)
	{
		mSampler[i].Terminate();
	}
	mVertexBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

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

	mSampler[0].BindPS(0);
	mTexture[0].BindPS(0);

	Matrix4 matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	TransformData transformData;

	//Set Matrix then draw Cube
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mVertexBuffer.Render();

	//Update Matrix, then draw a second cube
	mSampler[1].BindPS(0);
	mTexture[1].BindPS(0);
	matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 3.0f, 0.0f, 3.0f });
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mVertexBuffer.Render();

	//Update Matrix, then draw a thrid cube
	mSampler[2].BindPS(0);
	mTexture[2].BindPS(0);
	matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ -3.0f, 0.0f, 3.0f });
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mVertexBuffer.Render();

	//Update Matrix, then draw plane
	//mSampler[3].BindPS(3);
	//mTexture[3].BindPS(3);
	//matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 0.0f, -3.0f, 2.0f });
	//transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	//mConstantBuffer.Update(transformData);
	//mVertexBuffer.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);
	ImGui::End();
}

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands