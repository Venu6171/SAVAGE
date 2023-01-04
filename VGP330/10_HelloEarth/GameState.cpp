#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 1.5f, -45.0f });

	rings = 200;
	slices = 200;
	radius = 1.0f;

	mMesh = MeshBuilder::CreateSphere(rings, slices, radius);
	mCloudMesh = MeshBuilder::CreateSpherePX(rings, slices, radius);
	mSkyBoxMesh = MeshBuilder::CreateSkyBox(500.0f);

	mMeshBuffer.Initialize(mMesh);
	mCloudMeshBuffer.Initialize(mCloudMesh);
	mSkyBoxMeshBuffer.Initialize(mSkyBoxMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/EarthEffects.fx", Vertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/EarthEffects.fx");

	mTextureVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mTexturePixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");

	mMoonVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mMoonPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::Additive);

	mTexture[0].Initialize("../../Assets/Images/earth.jpg");
	mTexture[1].Initialize("../../Assets/Images/earth_spec.jpg");
	mTexture[2].Initialize("../../Assets/Images/earth_bump.jpg");
	mTexture[3].Initialize("../../Assets/Images/earth_normal.jpg");
	mTexture[4].Initialize("../../Assets/Images/earth_lights.jpg");

	mCloudTexture.Initialize("../../Assets/Images/earth_clouds.jpg");
	mMoonTexture.Initialize("../../Assets/Images/Moon.png");
	mSkyboxTexture.Initialize("../../Assets/Images/Galaxy.png");

	mTransformBuffer.Initialize();
	mTextureBuffer.Initialize();

	mLightBuffer.Initialize();
	mMoonLightBuffer.Initialize();

	mMaterialBuffer.Initialize();
	mMoonMaterialBuffer.Initialize();

	mDirectionalLight.ambient = SAVAGE::Graphics::Colors::Black;;
	mDirectionalLight.diffuse = SAVAGE::Graphics::Colors::White;
	mDirectionalLight.specular = SAVAGE::Graphics::Colors::White;
	mDirectionalLight.direction = { 1.0f, -1.0f, 1.0f };

	mMoonDirectionalLight.ambient = Colors::White;

	mMaterial.ambient = SAVAGE::Graphics::Colors::Black;
	mMaterial.diffuse = SAVAGE::Graphics::Colors::LightGray;
	mMaterial.specular = SAVAGE::Graphics::Colors::DimGray;

	mMoonMaterial.ambient = Colors::White;
}

void GameState::Terminate()
{
	mSkyboxTexture.Terminate();
	mMoonTexture.Terminate();
	mCloudTexture.Terminate();
	for (auto& tex : mTexture)
	{
		tex.Terminate();
	}

	mBlendState.Terminate();
	mSampler.Terminate();

	mMoonVertexShader.Terminate();
	mMoonPixelShader.Terminate();

	mTextureVertexShader.Terminate();
	mTexturePixelShader.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mSkyBoxMeshBuffer.Terminate();
	mCloudMeshBuffer.Terminate();
	mMeshBuffer.Terminate();

	mMoonMaterialBuffer.Terminate();
	mMoonLightBuffer.Terminate();

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
		mRotation.y += deltaTime;
		mCloudRotation.y += deltaTime;
		mMoonRotation.y += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mRotation.y -= deltaTime;
		mCloudRotation.y -= deltaTime;
		mMoonRotation.y -= deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mRotation.x += deltaTime;
		mCloudRotation.x += deltaTime;
		mMoonRotation.x += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mRotation.x -= deltaTime;
		mCloudRotation.x -= deltaTime;
		mMoonRotation.x -= deltaTime;
	}

	mRotation.y -= 0.00001f;
	mCloudRotation.y -= 0.0001f;
	mMoonRotation.y -= 0.0001f;

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
	matView = mCamera.GetViewMatrix();
	matProj = mCamera.GetProjectionMatrix();

	textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTextureBuffer.Update(textureData);

	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();

	mSkyBoxMeshBuffer.Render();

	// Earth
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

	mTexture[0].BindVS(0);
	mTexture[0].BindPS(0);

	mTexture[1].BindVS(1);
	mTexture[1].BindPS(1);

	mTexture[2].BindVS(2);
	mTexture[2].BindPS(2);

	mTexture[3].BindVS(3);
	mTexture[3].BindPS(3);

	mTexture[4].BindVS(4);
	mTexture[4].BindPS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	matWorld =
		Matrix4::Scaling({ 10.0f, 10.0f, 10.0f }) *
		Matrix4::RotationX(mRotation.x) *
		Matrix4::RotationY(mRotation.y) *
		Matrix4::Translation(mPosition);

	transformData.world = SAVAGE::Math::Transpose(matWorld);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();

	// Clouds
	mTextureBuffer.BindVS(0);
	mTextureBuffer.BindPS(0);

	mCloudTexture.BindVS(0);
	mCloudTexture.BindPS(0);

	matWorld =
		Matrix4::Scaling({ 11.6f, 11.6f, 11.6f }) *
		Matrix4::RotationX(mCloudRotation.x) *
		Matrix4::RotationY(mCloudRotation.y) *
		Matrix4::Translation(mPosition);

	textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTextureBuffer.Update(textureData);

	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();

	mBlendState.Set();
	mCloudMeshBuffer.Render();
	BlendState::ClearState();

	// Moon
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mMoonLightBuffer.Update(mMoonDirectionalLight);
	mMoonLightBuffer.BindVS(1);
	mMoonLightBuffer.BindPS(1);

	mMoonMaterialBuffer.Update(mMoonMaterial);
	mMoonMaterialBuffer.BindVS(2);
	mMoonMaterialBuffer.BindPS(2);

	mMoonTexture.BindVS(0);
	mMoonTexture.BindPS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	matWorld =
		Matrix4::Scaling({ 3.0f, 3.0f, 3.0f }) *
		Matrix4::RotationX(mMoonRotation.x) *
		Matrix4::RotationY(mMoonRotation.y) *
		Matrix4::Translation(mPosition + Vector3{ 25.0f, 10.0f, 15.0f }) * Matrix4::RotationY(mMoonRotation.y * 0.5f);

	transformData.world = SAVAGE::Math::Transpose(matWorld);
	transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	mMoonVertexShader.Bind();
	mMoonPixelShader.Bind();
	mMeshBuffer.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	mMaterial.power = 20.0f;

	// Earth Light
	ImGui::DragFloat3("LightDirection", &mDirectionalLight.direction.x, 0.1f);
	ImGui::DragFloat3("EarthRotation", (float*)&mRotation, 0.01f);

	ImGui::ColorEdit3("EarthLightAmbient", &mDirectionalLight.ambient.x);
	ImGui::ColorEdit3("EarthLightDiffuse", &mDirectionalLight.diffuse.x);
	ImGui::ColorEdit3("EarthLightSpecular", &mDirectionalLight.specular.x);

	// Earth Material
	ImGui::ColorEdit3("EarthMaterialAmbient", &mMaterial.ambient.x);
	ImGui::ColorEdit3("EarthMaterialDiffuse", &mMaterial.diffuse.x);
	ImGui::ColorEdit3("EarthMaterialSpecular", &mMaterial.specular.x);

	ImGui::SliderFloat("EarthMaterialPower", &mMaterial.power, 1.0f, 100.0f);

	ImGui::DragFloat3("CloudRotation", (float*)&mCloudRotation, 0.01f);
	ImGui::DragFloat3("MoonRotation", (float*)&mMoonRotation, 0.01f);

	// Moon Light
	ImGui::ColorEdit3("MoonLightAmbient", &mMoonDirectionalLight.ambient.x);

	// Moon Material
	ImGui::ColorEdit3("MoonMaterialAmbient", &mMoonMaterial.ambient.x);

	ImGui::End();
}

// wvp -> WorldMatrix, ViewMatrix and ProjectionMatrix

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands