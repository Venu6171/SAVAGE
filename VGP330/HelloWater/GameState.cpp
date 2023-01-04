#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	mSkyBoxMesh = MeshBuilder::CreateSkyBox(500.0f);
	mPlaneMesh = MeshBuilder::CreatePlanePX(15, 15, 1.0f, 2.0f);

	mDefaultCamera.SetPosition({ 0.0f, 10.0f, -10.0f });

	mPlaneMeshBuffer.Initialize(mPlaneMesh);
	mSkyBoxMeshBuffer.Initialize(mSkyBoxMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/WaterShader.fx", VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/WaterShader.fx");

	mTextureVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mTexturePixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	mPlaneNormalTexture.Initialize("../../Assets/Images/Water_Normal.png");
	mPlaneReflectionTexture.Initialize("../../Assets/Images/Water_Reflection.png");
	mPlaneRefractionTexture.Initialize("../../Assets/Images/Water_Refraction.png");
	mSkyboxTexture.Initialize("../../Assets/Images/Galaxy.png");

	mWaterBuffer.Initialize();
	mSkyBoxBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });

	mMaterial.ambient = Colors::White;
	mMaterial.diffuse = Colors::White;
	mMaterial.specular = Colors::White;
}

void GameState::Terminate()
{
	mPlaneNormalTexture.Terminate();
	mPlaneReflectionTexture.Terminate();
	mPlaneRefractionTexture.Terminate();
	mSkyboxTexture.Terminate();

	mSampler.Terminate();

	mTextureVertexShader.Terminate();
	mTexturePixelShader.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mSkyBoxMeshBuffer.Terminate();
	mPlaneMeshBuffer.Terminate();

	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();

	mSettingsBuffer.Terminate();
	mSkyBoxBuffer.Terminate();
	mWaterBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

	auto inputSystem = InputSystem::Get();

	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mDefaultCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mDefaultCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mDefaultCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mDefaultCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mDefaultCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mDefaultCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	Matrix4 matView = mDefaultCamera.GetViewMatrix();
	Matrix4 matProj = mDefaultCamera.GetProjectionMatrix();
	Matrix4 matWorld = Matrix4::Identity();

	TextureData textureData;
	mSkyBoxBuffer.BindVS(0);
	mSkyboxTexture.BindPS(0);

	matWorld = Matrix4::Translation(mDefaultCamera.GetPosition());
	textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mSkyBoxBuffer.Update(textureData);

	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();

	mSkyBoxMeshBuffer.Render();

	// Plane
	WaterData waterData;
	waterData.viewPosition = mDefaultCamera.GetPosition();

	mWaterBuffer.BindVS(0);
	mWaterBuffer.BindPS(0);

	mSettingsBuffer.BindVS(1);
	mSettingsBuffer.BindPS(1);

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mPlaneReflectionTexture.BindPS(0);
	mPlaneRefractionTexture.BindPS(1);
	mPlaneNormalTexture.BindPS(2);

	matWorld = SAVAGE::Math::Transpose(Matrix4::Identity());
	waterData.wvp = SAVAGE::Math::Transpose(matView * matProj);
	waterData.reflectionMatrix = SAVAGE::Math::Transpose(matWorld * matView * matView);
	mWaterBuffer.Update(waterData);
	mSettingsBuffer.Update(mSettingsData);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mPlaneMeshBuffer.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	ImGui::Text("Water: %f", mSettingsData.waterTranslation);

	mMaterial.power = 20.0f;

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("LightDirection", &mDirectionalLight.direction.x, 0.1f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit3("BasketBallLightAmbient", &mDirectionalLight.ambient.x);
		ImGui::ColorEdit3("BasketBallLightDiffuse", &mDirectionalLight.diffuse.x);
		ImGui::ColorEdit3("BasketBallLightSpecular", &mDirectionalLight.specular.x);
	}

	// Earth Material
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit3("BasketBallMaterialAmbient", &mMaterial.ambient.x);
		ImGui::ColorEdit3("BasketBallMaterialDiffuse", &mMaterial.diffuse.x);
		ImGui::ColorEdit3("BasketBallMaterialSpecular", &mMaterial.specular.x);
	}

	ImGui::SliderFloat("BasketBallMaterialPower", &mMaterial.power, 1.0f, 100.0f);
	ImGui::SliderFloat("WaterTranslation", &mSettingsData.waterTranslation, 1.0f, 100.0f);
	ImGui::SliderFloat("Reflect&Refract Scale", &mSettingsData.reflectRefractScale, 1.0f, 100.0f);


	ImGui::End();
}