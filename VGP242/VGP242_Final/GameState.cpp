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
	//mMesh = MeshBuilder::CreateCylinderPX(2, 10, 4.0f, 1.0f);

	mMesh = MeshBuilder::CreateSpherePX(200, 200, 1.0f);
	mSkyboxMesh = MeshBuilder::CreateSkyBox(500.0f);

	mMeshBuffer.Initialize(mMesh);
	mSkyBoxMeshBuffer.Initialize(mSkyboxMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");

	mTexture[0].Initialize("../../Assets/Images/Sun.png");
	mTexture[1].Initialize("../../Assets/Images/Mercury.png");
	mTexture[2].Initialize("../../Assets/Images/Venus.png");
	mTexture[3].Initialize("../../Assets/Images/EarthDay.png");
	mTexture[4].Initialize("../../Assets/Images/Mars.png");
	mTexture[5].Initialize("../../Assets/Images/Jupiter.png");
	mTexture[6].Initialize("../../Assets/Images/Saturn.png");
	mTexture[7].Initialize("../../Assets/Images/Uranus.png");
	mTexture[8].Initialize("../../Assets/Images/Neptune.png");

	mSkyboxTexture.Initialize("../../Assets/Images/Universe.png");

	mConstantBuffer.Initialize();

	mCamera.SetPosition({ 0.0f, 20.0f, -200.0f });
	mCamera.SetDirection({ 0.0f, 00.0f, 100.0f });
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	for (int i = 0; i < std::size(mTexture); ++i)
	{
		mTexture[i].Terminate();
	}

	mSkyboxTexture.Terminate();

	mSkyBoxMeshBuffer.Terminate();
	mMeshBuffer.Terminate();

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

	mTexture[0].BindPS(0);

	// SUN
	Matrix4 matWorld = Matrix4::Scaling({ 30.0f, 30.0f, 30.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	TransformData transformData;

	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();

	// SkyBox 
	mSkyboxTexture.BindPS(0);
	matWorld = Matrix4::Translation(mCamera.GetPosition());
	matView = mCamera.GetViewMatrix();
	matProj = mCamera.GetProjectionMatrix();

	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mSkyBoxMeshBuffer.Render();

	//Mercury
	mTexture[1].BindPS(0);
	matWorld = Matrix4::Scaling({ 1.0f, 1.0f, 1.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 35.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 0.5f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();

	//Venus
	mTexture[2].BindPS(0);
	matWorld = Matrix4::Scaling({ 5.0f, 5.0f, 5.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 43.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 0.02f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();

	//Earth
	mTexture[3].BindPS(0);
	matWorld = Matrix4::Scaling({ 4.0f, 4.0f, 4.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 55.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 0.9f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();


	//Mars
	mTexture[4].BindPS(0);
	matWorld = Matrix4::Scaling({ 2.0f, 2.0f, 2.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 65.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 1.03f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();

	//Jupiter
	mTexture[5].BindPS(0);
	matWorld = Matrix4::Scaling({ 9.0f, 9.0f, 9.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 80.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 1.41f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();


	//Saturn
	mTexture[6].BindPS(0);
	matWorld = Matrix4::Scaling({ 7.0f, 7.0f, 7.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 100.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 1.45f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();


	//Uranus
	mTexture[7].BindPS(0);
	matWorld = Matrix4::Scaling({ 4.0f, 4.0f, 4.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 115.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 1.27f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();


	//Neptune
	mTexture[8].BindPS(0);
	matWorld = Matrix4::Scaling({ 4.4f, 4.5f, 4.5f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation(mPosition + Vector3{ 125.0f, 0.0f, 0.0f }) * Matrix4::RotationY(mRotation.y * 1.26f);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mMeshBuffer.Render();
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