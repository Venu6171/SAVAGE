#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);

	mCamera.SetPosition({ 2.0f, 0.0f, -8.0f });

	rings = 20;
	slices = 20;
	radius = 1.0f;

	mMesh = MeshBuilder::CreateSpherePN(rings, slices, radius);
	mMeshBuffer.Initialize(mMesh);

	const wchar_t* shaderFileNames[] =
	{
		L"../../Assets/Shaders/DoFlatShading.fx",
		L"../../Assets/Shaders/DoGouraudShading.fx",
		L"../../Assets/Shaders/DoPhongShading.fx"
	};

	for (size_t i = 0; i < std::size(shaderFileNames); ++i)
	{
		mVertexShader[i].Initialize(shaderFileNames[i], VertexPN::Format);
		mPixelShader[i].Initialize(shaderFileNames[i]);
	}

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.ambient = SAVAGE::Graphics::Colors::Black;
	mDirectionalLight.diffuse = SAVAGE::Graphics::Colors::White;
	mDirectionalLight.specular = SAVAGE::Graphics::Colors::Black;
	mDirectionalLight.direction = { 1.0f, -1.0f, 1.0f };

	mMaterial.ambient = SAVAGE::Graphics::Colors::Black;
	mMaterial.diffuse = SAVAGE::Graphics::Colors::Red;
	mMaterial.specular = SAVAGE::Graphics::Colors::Black;

	mPosition.z = 5.0f;
	mPosition.y = 0.0f;
}

void GameState::Terminate()
{
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mMeshBuffer.Terminate();
	for (auto& vs : mVertexShader)
	{
		vs.Terminate();
	}
	for (auto& ps : mPixelShader)
	{
		ps.Terminate();
	}
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
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();

	TransformData transformData;
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	const float xOffsets[] = { -2.0f, 2.0f, 6.0f };
	for (size_t i = 0; i < std::size(xOffsets); ++i)
	{
		Matrix4 matWorld =
			Matrix4::RotationX(mRotation.x) *
			Matrix4::RotationY(mRotation.y) *
			Matrix4::Translation(xOffsets[i], 0.0f, 0.0f);

		transformData.world = SAVAGE::Math::Transpose(matWorld);
		transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
		mTransformBuffer.Update(transformData);

		mVertexShader[i].Bind();
		mPixelShader[i].Bind();
		mMeshBuffer.Render();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);

	// Light
	if (ImGui::DragFloat3("LightDirection", &mDirectionalLight.direction.x, 0.1f))
	{
		mDirectionalLight.direction = SAVAGE::Math::Normalize(mDirectionalLight.direction);
	}

	ImGui::ColorEdit3("LightAmbient", &mDirectionalLight.ambient.x);
	ImGui::ColorEdit3("LightDiffuse", &mDirectionalLight.diffuse.x);
	ImGui::ColorEdit3("LightSpecular", &mDirectionalLight.specular.x);

	//ImGui::SliderFloat("LightPadding", &mDirectionalLight.padding, 0.0f, 50.0f);

	// Material
	ImGui::ColorEdit3("MaterialAmbient", &mMaterial.ambient.x);
	ImGui::ColorEdit3("MaterialDiffuse", &mMaterial.diffuse.x);
	ImGui::ColorEdit3("MaterialSpecular", &mMaterial.specular.x);

	ImGui::SliderFloat("MaterialPower", &mMaterial.power, 1.0f, 100.0f);
	//ImGui::SliderFloat3("MaterialPadding1", &mMaterial.padding[0], 0.0f, 50.0f);

	ImGui::End();
}

// wvp -> WorldMatrix, ViewMatrix and ProjectionMatrix

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands