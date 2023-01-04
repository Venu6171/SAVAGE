#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);

	mCamera.SetPosition({ 0.0f, 3.5f, -10.0f });
	mCamera.SetNearPlane(0.001f);

	mPlaneMesh = MeshBuilder::CreatePlanePX(5, 5, 1.0f);
	mGlassMesh = MeshBuilder::CreateSpherePX(256, 256, 1.0f);
	mGlitchMesh = MeshBuilder::CreateSpherePX(256, 256, 1.0f);

	mModelLoader.LoadObj(L"../../Assets/Models/trident_a10/trident_a10.obj", 0.01f, mModelMesh);

	mPlaneMeshBuffer.Initialize(mPlaneMesh);
	mModelMeshBuffer.Initialize(mModelMesh);
	mGlassMeshBuffer.Initialize(mGlassMesh);
	mGlitchMeshBuffer.Initialize(mGlitchMesh);

	mPlaneTextureVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPlaneTexturePixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mGlassVertexShader.Initialize(L"../../Assets/Shaders/GlassEffect.fx", VertexPX::Format);
	mGlassPixelShader.Initialize(L"../../Assets/Shaders/GlassEffect.fx");

	mGlitchVertexShader.Initialize(L"../../Assets/Shaders/Glitch.fx", VertexPX::Format);
	mGlitchPixelShader.Initialize(L"../../Assets/Shaders/Glitch.fx");

	//mHologramVertexShader.Initialize(L"../../Assets/Shaders/Hologram.fx", Vertex::Format);
	//mHologramPixelShader.Initialize(L"../../Assets/Shaders/Hologram.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);

	mPlaneTexture.Initialize(L"../../Assets/Images/Ground.png.");

	mTexture.Initialize(L"../../Assets/Images/Texture.jpg");
	mNormalTexture.Initialize(L"../../Assets/Images/Normal.jpg");
	mRefractionTexture.Initialize(L"../../Assets/Images/Glass.jpg");

	mGlitchTexture.Initialize(L"../../Assets/Images/earth.jpg");

	mModelDiffuseTexture.Initialize(L"../../Assets/Models/trident_a10/trident_decal_diffuse.tif");
	mAlphaTexture.Initialize(L"../../Assets/Images/AlphaTexture.png");

	mTransformBuffer.Initialize();
	mGlassBuffer.Initialize();
	mGlitchDataBuffer.Initialize();
	mHologramDataBuffer.Initialize();
	mGlassSettingsBuffer.Initialize();
	mGlitchSettingsBuffer.Initialize();
	mHologramSettingsBuffer.Initialize();

	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.ambient = SAVAGE::Graphics::Colors::White;;
	mDirectionalLight.diffuse = SAVAGE::Graphics::Colors::Black;
	mDirectionalLight.specular = SAVAGE::Graphics::Colors::Black;
	mDirectionalLight.direction = { 1.0f, -1.0f, 1.0f };

	mMaterial.ambient = SAVAGE::Graphics::Colors::White;
	mMaterial.diffuse = SAVAGE::Graphics::Colors::Black;
	mMaterial.specular = SAVAGE::Graphics::Colors::Black;
}

void GameState::Terminate()
{
	mAlphaTexture.Terminate();
	mModelDiffuseTexture.Terminate();
	mGlitchTexture.Terminate();
	mNormalTexture.Terminate();
	mRefractionTexture.Terminate();
	mTexture.Terminate();
	mPlaneTexture.Terminate();

	mBlendState.Terminate();
	mSampler.Terminate();

	mHologramVertexShader.Terminate();
	mHologramPixelShader.Terminate();

	mGlitchVertexShader.Terminate();
	mGlitchPixelShader.Terminate();

	mPlaneTextureVertexShader.Terminate();
	mPlaneTexturePixelShader.Terminate();

	mGlassVertexShader.Terminate();
	mGlassPixelShader.Terminate();

	mGlitchMeshBuffer.Terminate();
	mGlassMeshBuffer.Terminate();
	mPlaneMeshBuffer.Terminate();
	mModelMeshBuffer.Terminate();

	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();

	mHologramSettingsBuffer.Terminate();
	mGlitchSettingsBuffer.Terminate();
	mGlassSettingsBuffer.Terminate();
	mHologramDataBuffer.Terminate();
	mGlitchDataBuffer.Terminate();
	mGlassBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mModelPosition.x -= 0.1f;
		mGlassRotation.y += deltaTime;
		mGlitchRotation.y += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mModelPosition.x += 0.1f;
		mGlassRotation.y -= deltaTime;
		mGlitchRotation.y -= deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mModelPosition.y += 0.1f;
		mGlassRotation.x += deltaTime;
		mGlitchRotation.x += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mModelPosition.y -= 0.1f;
		mGlassRotation.x -= deltaTime;
		mGlitchRotation.x -= deltaTime;
	}

	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	//mGlitchSettingsData.glitchEffect += 0.5 * deltaTime;

	if (mGlitchSettingsData.glitchEffect >= 6.0f)
	{
		mGlitchSettingsData.glitchEffect = 0.0f;
	}

}

void GameState::Render()
{
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matWorld = SAVAGE::Math::Matrix4::Identity();

	GlassData glassData;

	// Glass Earth
	mGlassBuffer.BindVS(0);
	mGlassBuffer.BindPS(0);

	mGlassSettingsBuffer.BindVS(1);
	mGlassSettingsBuffer.BindPS(1);

	mTexture.BindVS(0);
	mTexture.BindPS(0);

	mNormalTexture.BindVS(1);
	mNormalTexture.BindPS(1);

	mRefractionTexture.BindVS(2);
	mRefractionTexture.BindPS(2);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	matWorld =
		Matrix4::RotationX(mGlassRotation.x) *
		Matrix4::RotationY(mGlassRotation.y) *
		Matrix4::Translation(mGlassPosition);

	glassData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mGlassBuffer.Update(glassData);

	mGlassSettingsBuffer.Update(mGlassSettingsData);

	mGlassVertexShader.Bind();
	mGlassPixelShader.Bind();
	mPlaneMeshBuffer.Render();

	// Glitch Earth
	GlitchData glitchData;

	mGlitchDataBuffer.BindVS(0);
	mGlitchDataBuffer.BindPS(0);

	mGlitchSettingsBuffer.BindVS(1);
	mGlitchSettingsBuffer.BindPS(1);

	mGlitchTexture.BindVS(0);
	mGlitchTexture.BindPS(0);

	matWorld =
		Matrix4::RotationX(mGlitchRotation.x) *
		Matrix4::RotationY(mGlitchRotation.y) *
		Matrix4::Translation(mGlitchPosition);

	glitchData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mGlitchDataBuffer.Update(glitchData);

	mGlitchSettingsBuffer.Update(mGlitchSettingsData);

	mGlitchVertexShader.Bind();
	mGlitchPixelShader.Bind();
	mGlitchMeshBuffer.Render();

	// SpaceShip
	/*HologramData hologramData;
	hologramData.viewPosition = mCamera.GetPosition();

	mHologramDataBuffer.BindVS(0);
	mHologramDataBuffer.BindPS(0);

	mHologramSettingsBuffer.BindVS(1);
	mHologramSettingsBuffer.BindPS(1);

	mGlitchTexture.BindVS(0);
	mGlitchTexture.BindPS(0);

	mAlphaTexture.BindVS(1);
	mAlphaTexture.BindPS(1);

	matWorld =
		Matrix4::RotationX(mGlitchRotation.x) *
		Matrix4::RotationY(mGlitchRotation.y) *
		Matrix4::Translation(mGlitchPosition);

	hologramData.world = SAVAGE::Math::Transpose(matWorld);
	hologramData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mHologramDataBuffer.Update(hologramData);

	mHologramSettingsBuffer.Update(mHologramSettingsData);

	mHologramVertexShader.Bind();
	mHologramPixelShader.Bind();

	mBlendState.Set();
	mGlitchMeshBuffer.Render();
	BlendState::ClearState();*/

	// Plane
	/*TransformData transformData;
	transformData.viewPosition = mCamera.GetPosition();

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mPlaneTexture.BindVS(0);
	mPlaneTexture.BindPS(0);

	matWorld =
		Matrix4::RotationX(mPlaneRotation.x) *
		Matrix4::RotationY(mPlaneRotation.y) *
		Matrix4::Translation(mPlanePosition);

	transformData.world = SAVAGE::Math::Transpose(matWorld);
	transformData.wvp[0] = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	mPlaneTextureVertexShader.Bind();
	mPlaneTexturePixelShader.Bind();
	mPlaneMeshBuffer.Render();*/
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	mMaterial.power = 20.0f;

	ImGui::DragFloat3("LightDirection", &mDirectionalLight.direction.x, 0.1f);
	ImGui::DragFloat3("PlaneRotation", (float*)&mPlaneRotation, 0.01f);

	ImGui::ColorEdit3("PlaneLightAmbient", &mDirectionalLight.ambient.x);
	ImGui::ColorEdit3("PlaneLightDiffuse", &mDirectionalLight.diffuse.x);
	ImGui::ColorEdit3("PlaneLightSpecular", &mDirectionalLight.specular.x);

	ImGui::ColorEdit3("PlaneMaterialAmbient", &mMaterial.ambient.x);
	ImGui::ColorEdit3("PlaneMaterialDiffuse", &mMaterial.diffuse.x);
	ImGui::ColorEdit3("PlaneMaterialSpecular", &mMaterial.specular.x);

	ImGui::SliderFloat("MaterialPower", &mMaterial.power, 1.0f, 100.0f);

	ImGui::DragFloat("RefractionScale", &mGlassSettingsData.refractionScale, 0.001f);
	ImGui::DragFloat("ChromoAmountX", &mGlitchSettingsData.chromoAmountX, 0.1f);
	ImGui::DragFloat("ChromoAmountY", &mGlitchSettingsData.chromoAmountY, 0.1f);
	ImGui::DragFloat("GlitchEffect", &mGlitchSettingsData.glitchEffect, 0.1f);
	ImGui::DragFloat("DesaturationAmount", &mGlitchSettingsData.desaturationAmount, 0.1f);

	//ImGui::DragFloat("Scale", &mHologramSettingsData.scale, 0.1f);
	//ImGui::DragFloat("ScrollSpeed", &mHologramSettingsData.scrollSpeed, 0.1f);
	//ImGui::DragFloat("GlowIntensity", &mHologramSettingsData.glowIntensity, 0.1f);
	//ImGui::DragFloat("GlitchSpeed", &mHologramSettingsData.glitchSpeed, 0.1f);

	ImGui::End();
}

// wvp -> WorldMatrix, ViewMatrix and ProjectionMatrix

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands