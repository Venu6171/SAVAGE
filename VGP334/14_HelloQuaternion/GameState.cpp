#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	//GraphicsSystem::Get()->SetClearColor(Colors::Gray);

	mCamera.SetPosition({ 0.0f, 1.5f, -45.0f });

	rings = 200;
	slices = 200;
	radius = 1.0f;

	mSkyBoxMesh = MeshBuilder::CreateSkyBox(500.0f);

	mModelLoader.LoadObj(L"../../Assets/Models/trident_a10/trident_a10.obj", 0.01f, mModelMesh);

	mSkyBoxMeshBuffer.Initialize(mSkyBoxMesh);
	mModelMeshBuffer.Initialize(mModelMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mTextureVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mTexturePixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::Additive);

	mSkyboxTexture.Initialize("../../Assets/Images/Galaxy.png");

	mModelDiffuseTexture.Initialize(L"../../Assets/Models/trident_a10/trident_decal_diffuse.tif");
	mModelSpecularTexture.Initialize(L"../../Assets/Models/trident_a10/trident_specular.tif");
	mModelBumpTexture.Initialize(L"../../Assets/Models/trident_a10/trident_bump.tif");

	mTransformBuffer.Initialize();
	mTextureBuffer.Initialize();

	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.ambient = SAVAGE::Graphics::Colors::White;;
	mDirectionalLight.diffuse = SAVAGE::Graphics::Colors::White;
	mDirectionalLight.specular = SAVAGE::Graphics::Colors::Black;
	mDirectionalLight.direction = { 1.0f, -1.0f, 1.0f };

	mMoonDirectionalLight.ambient = Colors::White;

	mMaterial.ambient = SAVAGE::Graphics::Colors::White;
	mMaterial.diffuse = SAVAGE::Graphics::Colors::White;
	mMaterial.specular = SAVAGE::Graphics::Colors::Black;

	mMoonMaterial.ambient = Colors::White;
}

void GameState::Terminate()
{
	mModelDiffuseTexture.Terminate();
	mModelSpecularTexture.Terminate();
	mModelBumpTexture.Terminate();

	mSkyboxTexture.Terminate();

	mBlendState.Terminate();
	mSampler.Terminate();

	mTextureVertexShader.Terminate();
	mTexturePixelShader.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mModelMeshBuffer.Terminate();
	mSkyBoxMeshBuffer.Terminate();

	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();

	mTextureBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mModelRotationQuaternion.y += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mModelRotationQuaternion.y -= deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mModelRotationQuaternion.x += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mModelRotationQuaternion.x -= deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::LCONTROL))
	{
		mModelRotationQuaternion.z += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
	{
		mModelRotationQuaternion.z -= deltaTime;
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
	Matrix4 matWorld = SAVAGE::Math::Matrix4::Identity();

	TextureData textureData;

	// SkyBox
	mTextureBuffer.BindVS(0);
	mTextureBuffer.BindPS(0);

	mSkyboxTexture.BindVS(0);
	mSkyboxTexture.BindPS(0);

	matWorld = Matrix4::Translation(mCamera.GetPosition());

	textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTextureBuffer.Update(textureData);

	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();
	mSkyBoxMeshBuffer.Render();

	// SpaceShip
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

	mModelDiffuseTexture.BindVS(0);
	mModelDiffuseTexture.BindPS(0);

	mModelSpecularTexture.BindVS(1);
	mModelSpecularTexture.BindPS(1);

	mModelBumpTexture.BindVS(2);
	mModelBumpTexture.BindPS(2);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	matWorld =
		Matrix4::RotationQuaternion(Normalize(mModelRotationQuaternion)) *
		Matrix4::Translation(mModelPosition);

	transformData.world = SAVAGE::Math::Transpose(matWorld);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	mVertexShader.Bind();
	mPixelShader.Bind();
	mModelMeshBuffer.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	mMaterial.power = 20.0f;

	// Earth Light
	if (ImGui::DragFloat3("LightDirection", &mDirectionalLight.direction.x, 0.1f))
	{
		mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
	}
	ImGui::DragFloat3("CameraRotation", &mCameraRotation.x, 0.01f);
	ImGui::DragFloat4("SpaceShipRotation", &mModelRotationQuaternion.x, 0.01f);

	ImGui::ColorEdit3("SpaceShipLightAmbient", &mDirectionalLight.ambient.x);
	ImGui::ColorEdit3("SpaceShipLightDiffuse", &mDirectionalLight.diffuse.x);
	ImGui::ColorEdit3("SpaceShipLightSpecular", &mDirectionalLight.specular.x);

	// Earth Material
	ImGui::ColorEdit3("SpaceShipMaterialAmbient", &mMaterial.ambient.x);
	ImGui::ColorEdit3("SpaceShipMaterialDiffuse", &mMaterial.diffuse.x);
	ImGui::ColorEdit3("SpaceShipMaterialSpecular", &mMaterial.specular.x);

	ImGui::SliderFloat("EarthMaterialPower", &mMaterial.power, 1.0f, 100.0f);

	ImGui::End();
}

// wvp -> WorldMatrix, ViewMatrix and ProjectionMatrix

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands