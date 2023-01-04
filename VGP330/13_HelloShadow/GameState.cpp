#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::Gray);

	mDefaultCamera.SetPosition({ 0.0f, 15.0f, -10.0f });
	mDefaultCamera.SetNearPlane(0.001f);
	mDefaultCamera.Pitch(20.0f);

	mLightCamera.SetPosition({ -15.0f, 10.0f, -10.0f });
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;

	rings = 256;
	slices = 256;
	radius = 1.0f;

	constexpr uint32_t depthMapSize = 4096;
	mDepthRenderTarget.Initialize(depthMapSize, depthMapSize, Texture::Format::RGBA_F32);

	mBaseRenderTarget.Initialize(
		graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight()
	);

	mBrightRenderTarget.Initialize(
		graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight()
	);

	mPlaneMesh = MeshBuilder::CreatePlane(30, 30, 1.0f);
	mBallMesh = MeshBuilder::CreateSphere(rings, slices, radius);

	//mModelLoader.LoadObj(L"../../Assets/Models/trident_a10/trident_a10.obj", 0.01f, mModelMesh);

	mPlaneMeshBuffer.Initialize(mPlaneMesh);
	mBallMeshBuffer.Initialize(mBallMesh);
	//mModelMeshBuffer.Initialize(mModelMesh);

	//mPlaneMesh = MeshBuilder::CreateSphere(rings, slices, radius);
	//mTextureMesh = MeshBuilder::CreateSpherePX(rings, slices, radius);
	//mSkyBoxMesh = MeshBuilder::CreateSkyBox(500.0f);

	//mPlaneMeshBuffer.Initialize(mPlaneMesh);
	//mTextureMeshBuffer.Initialize(mTextureMesh);
	//mSkyBoxMeshBuffer.Initialize(mSkyBoxMesh);

	// Shader
	/*mVertexShader.Initialize(L"../../Assets/Shaders/EarthEffects.fx", Vertex::Format);
	  mPixelShader.Initialize(L"../../Assets/Shaders/EarthEffects.fx");

	mBloomVertexShader.Initialize(L"../../Assets/Shaders/Bloom.fx", VertexPX::Format);
	mBloomPixelShader.Initialize(L"../../Assets/Shaders/Bloom.fx");

	mMoonVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mMoonPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mTextureVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mTexturePixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");*/

	mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mDepthMapVertexShader.Initialize(L"../../Assets/Shaders/DepthMap.fx", Vertex::Format);
	mDepthMapPixelShader.Initialize(L"../../Assets/Shaders/DepthMap.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	mPlaneTexture.Initialize("../../Assets/Images/Ground.png");
	mBallTexture.Initialize("../../Assets/Images/Basketball.png");

	//mModelDiffuseTexture.Initialize(L"../../Assets/Models/trident_a10/trident_decal_diffuse.tif");
	//mModelSpecularTexture.Initialize(L"../../Assets/Models/trident_a10/trident_specular.tif");
	//mModelBumpTexture.Initialize(L"../../Assets/Models/trident_a10/trident_bump.tif");

	mDepthMapTransformBuffer.Initialize();
	mTransformBuffer.Initialize();
	mTextureBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mLightBuffer.Initialize();
	mMoonLightBuffer.Initialize();

	mMaterialBuffer.Initialize();
	mMoonMaterialBuffer.Initialize();

	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });

	mMaterial.ambient = Colors::White;
	mMaterial.diffuse = Colors::White;
	mMaterial.specular = Colors::White;

	//mScreenMesh = MeshBuilder::CreateScreenMesh();
	//mScreenMeshBuffer.Initialize(mScreenMesh);
}

void GameState::Terminate()
{
	mScreenMeshBuffer.Terminate();
	mModelDiffuseTexture.Terminate();
	mModelSpecularTexture.Terminate();
	mModelBumpTexture.Terminate();
	mBallTexture.Terminate();
	mPlaneTexture.Terminate();
	mSkyboxTexture.Terminate();

	mSampler.Terminate();

	mDepthMapVertexShader.Terminate();
	mDepthMapPixelShader.Terminate();

	mTextureVertexShader.Terminate();
	mTexturePixelShader.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mBallMeshBuffer.Terminate();
	mSkyBoxMeshBuffer.Terminate();
	mTextureMeshBuffer.Terminate();
	mPlaneMeshBuffer.Terminate();
	mModelMeshBuffer.Terminate();

	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mMoonLightBuffer.Terminate();
	mMoonMaterialBuffer.Terminate();

	mSettingsBuffer.Terminate();
	mTextureBuffer.Terminate();
	mTransformBuffer.Terminate();
	mDepthMapTransformBuffer.Terminate();

	mBrightRenderTarget.Terminate();
	mBaseRenderTarget.Terminate();
	mDepthRenderTarget.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mBallRotation.y += deltaTime;
		mModelRotation.y += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mBallRotation.y -= deltaTime;
		mModelRotation.y -= deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mBallRotation.x += deltaTime;
		mModelRotation.x += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mBallRotation.x -= deltaTime;
		mModelRotation.x -= deltaTime;
	}

	if (inputSystem->IsKeyDown(KeyCode::SPACE))
	{
		mModelPosition.z += 0.1f;
	}

	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mActiveCamera->Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mActiveCamera->Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mActiveCamera->Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mActiveCamera->Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mActiveCamera->Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mActiveCamera->Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	// Synchronize the light camera to the directional light (cannot look straight up or down)
	mLightCamera.SetDirection(mDirectionalLight.direction);
	SAVAGE::Math::Vector3 newCameraPosition = -mLightCamera.GetDirection() * 100.0f;
	mLightCamera.SetPosition(newCameraPosition);

	//if (inputSystem->IsKeyPressed(KeyCode::ONE))
	//{
	//	mActiveCamera = &mDefaultCamera;
	//}
	//if (inputSystem->IsKeyPressed(KeyCode::TWO))
	//{
	//	mActiveCamera = &mLightCamera;
	//}
}

void GameState::Render()
{
	// Record scene depths from light source
	RenderDepthMap();

	// Render Scene with shadow in it
	RenderScene();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useLightCamera = (mActiveCamera == &mLightCamera);
		if (ImGui::Checkbox("Use Light Camera", &useLightCamera))
		{
			mActiveCamera = useLightCamera ? &mLightCamera : &mDefaultCamera;
		}

		//ImGui::DragFloat("Light Camera Distance", &mLightCameraDistance, 1.0f);
	}

	mMaterial.power = 20.0f;

	ImGui::DragFloat3("BasketBallRotation", (float*)&mBallRotation, 0.01f);

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

	ImGui::Image(mDepthRenderTarget.GetRawData(), { 200.0f, 200.0f });

	bool specular = mSettingsData.useSpecularMap == 1;
	if (ImGui::Checkbox("useSpecularMap", &specular))
	{
		mSettingsData.useSpecularMap = specular ? 1 : 0;
	}

	bool normal = mSettingsData.useNormalMap == 1;
	if (ImGui::Checkbox("useNormalMap", &normal))
	{
		mSettingsData.useNormalMap = normal ? 1 : 0;
	}

	bool bump = mSettingsData.useBumpMap == 1;
	if (ImGui::Checkbox("useBumpMap", &bump))
	{
		mSettingsData.useBumpMap = bump ? 1 : 0;
	}

	ImGui::End();
}

void GameState::RenderDepthMap()
{
	mDepthRenderTarget.BeginRender(Colors::Black);

	// Draw all the shadow casters using the light camera
	Matrix4 matView = mLightCamera.GetViewMatrix();
	Matrix4 matProj = mLightCamera.GetProjectionMatrix();

	TextureData textureData;
	mDepthMapTransformBuffer.BindVS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	Matrix4 matWorld =
		Matrix4::RotationX(mBallRotation.x) *
		Matrix4::RotationY(mBallRotation.y) *
		Matrix4::Translation(mBallPosition);

	textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mDepthMapTransformBuffer.Update(textureData);

	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	mBallMeshBuffer.Render();

	//SpaceShip
	/*matWorld =
		Matrix4::RotationX(mModelRotation.x) *
		Matrix4::RotationY(mModelRotation.y) *
		Matrix4::Translation(mModelPosition);

	textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mDepthMapTransformBuffer.Update(textureData);

	mModelMeshBuffer.Render();*/

	mDepthRenderTarget.EndRender();
}

void GameState::RenderScene()
{
	//mBaseRenderTarget.BeginRender();

	Matrix4 matView = mActiveCamera->GetViewMatrix();
	Matrix4 matProj = mActiveCamera->GetProjectionMatrix();

	Matrix4 matViewLight = mLightCamera.GetViewMatrix();
	Matrix4 matProjLight = mLightCamera.GetProjectionMatrix();

	// Basketball
	TransformData transformData;
	transformData.viewPosition = mActiveCamera->GetPosition();

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mBallTexture.BindVS(0);
	mBallTexture.BindPS(0);

	mDepthRenderTarget.BindPS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	Matrix4 matWorld =
		Matrix4::RotationX(mBallRotation.x) *
		Matrix4::RotationY(mBallRotation.y) *
		Matrix4::Translation(mBallPosition);

	transformData.world = SAVAGE::Math::Transpose(matWorld);
	transformData.wvp[0] = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	transformData.wvp[1] = SAVAGE::Math::Transpose(matWorld * matViewLight * matProjLight);
	mTransformBuffer.Update(transformData);

	mSettingsBuffer.Update(mSettingsData);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mBallMeshBuffer.Render();

	// Plane
	mPlaneTexture.BindVS(0);
	mPlaneTexture.BindPS(0);

	transformData.world = SAVAGE::Math::Transpose(Matrix4::Identity());
	transformData.wvp[0] = SAVAGE::Math::Transpose(matView * matProj);
	transformData.wvp[1] = SAVAGE::Math::Transpose(matViewLight * matProjLight);
	mTransformBuffer.Update(transformData);

	mPlaneMeshBuffer.Render();

	// SpaceShip

	/*mModelDiffuseTexture.BindVS(0);
	mModelDiffuseTexture.BindPS(0);

	mModelSpecularTexture.BindVS(1);
	mModelSpecularTexture.BindPS(1);

	mModelBumpTexture.BindVS(2);
	mModelBumpTexture.BindPS(2);

	matWorld =
		Matrix4::RotationX(mModelRotation.x) *
		Matrix4::RotationY(mModelRotation.y) *
		Matrix4::Translation(mModelPosition);

	transformData.world = SAVAGE::Math::Transpose(matWorld);
	transformData.wvp[0] = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	transformData.wvp[1] = SAVAGE::Math::Transpose(matWorld * matViewLight * matProjLight);
	mTransformBuffer.Update(transformData);

	mModelMeshBuffer.Render();*/

	mDepthRenderTarget.UnbindPS(4);
}

void GameState::RenderBrightness()
{
	/*//mBrightRenderTarget.BeginRender();

	//Matrix4 matView = mActiveCamera->GetViewMatrix();
	//Matrix4 matProj = mActiveCamera->GetProjectionMatrix();
	//Matrix4 matWorld = SAVAGE::Math::Matrix4::Identity();

	//TextureData textureData;

	//// SkyBox

	//mTextureBuffer.BindVS(0);
	//mTextureBuffer.BindPS(0);

	//mSkyboxTexture.BindVS(0);
	//mSkyboxTexture.BindPS(0);

	//matWorld = Matrix4::Translation(mActiveCamera->GetPosition());
	//matView = mActiveCamera->GetViewMatrix();
	//matProj = mActiveCamera->GetProjectionMatrix();

	//textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	//mTextureBuffer.Update(textureData);

	//mTextureVertexShader.Bind();
	//mTexturePixelShader.Bind();

	//mSkyBoxMeshBuffer.Render();

	//// Earth

	//mTextureBuffer.BindVS(0);
	//mTextureBuffer.BindPS(0);

	//mBlankTexture.BindVS(0);
	//mBlankTexture.BindPS(0);

	//mSampler.BindVS(0);
	//mSampler.BindPS(0);

	//matWorld =
	//	Matrix4::Scaling({ 10.0f, 10.0f, 10.0f }) *
	//	Matrix4::RotationX(mRotation.x) *
	//	Matrix4::RotationY(mRotation.y) *
	//	Matrix4::Translation(mPosition) * Matrix4::RotationY(mRotation.y * 0.5f);

	//matView = mActiveCamera->GetViewMatrix();
	//matProj = mActiveCamera->GetProjectionMatrix();

	//textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	//mTextureBuffer.Update(textureData);

	//mTextureVertexShader.Bind();
	//mTexturePixelShader.Bind();
	//mPlaneMeshBuffer.Render();

	//// Moon
	//TransformData transformData;
	//transformData.viewPosition = mActiveCamera->GetPosition();

	//mTransformBuffer.BindVS(0);
	//mTransformBuffer.BindPS(0);

	//mMoonLightBuffer.Update(mMoonDirectionalLight);
	//mMoonLightBuffer.BindVS(1);
	//mMoonLightBuffer.BindPS(1);

	//mMoonMaterialBuffer.Update(mMoonMaterial);
	//mMoonMaterialBuffer.BindVS(2);
	//mMoonMaterialBuffer.BindPS(2);

	//mMoonTexture.BindVS(0);
	//mMoonTexture.BindPS(0);

	//mSampler.BindVS(0);
	//mSampler.BindPS(0);

	//matWorld =
	//	Matrix4::Scaling({ 3.0f, 3.0f, 3.0f }) *
	//	Matrix4::RotationX(mMoonRotation.x) *
	//	Matrix4::RotationY(mMoonRotation.y) *
	//	Matrix4::Translation(mPosition + Vector3{ 0.0f, 6.0f, -25.0f }) * Matrix4::RotationY(mMoonRotation.y * 0.9f);

	//transformData.world = SAVAGE::Math::Transpose(matWorld);
	//transformData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	//mTransformBuffer.Update(transformData);

	//mMoonVertexShader.Bind();
	//mMoonPixelShader.Bind();
	//mPlaneMeshBuffer.Render();

	//// Sun
	//mTextureBuffer.BindVS(0);
	//mTextureBuffer.BindPS(0);

	//mSunTexture.BindVS(0);
	//mSunTexture.BindPS(0);

	//matWorld =
	//	Matrix4::Scaling({ 30.0f, 30.0f, 30.0f }) *
	//	Matrix4::RotationX(mSunRotation.x) *
	//	Matrix4::RotationY(mSunRotation.y) *
	//	Matrix4::Translation(Vector3{ 0.0f, 0.0f, 70.0f });

	//textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	//mTextureBuffer.Update(textureData);

	//mTextureVertexShader.Bind();
	//mTexturePixelShader.Bind();
	//mTextureMeshBuffer.Render();*/

	//mBrightRenderTarget.EndRender();
}

void GameState::PostProcess()
{
	/*mBloomVertexShader.Bind();
	mBloomPixelShader.Bind();

	mSampler.BindPS(0);

	mBaseRenderTarget.BindPS(0);
	mBrightRenderTarget.BindPS(1);

	mScreenMeshBuffer.Render();

	mBaseRenderTarget.UnbindPS(0);
	mBrightRenderTarget.UnbindPS(1);*/

	//mVertexShader.Bind();
	//mPixelShader.Bind();
	//
	//mSampler.BindPS(0);
	//
	//mDepthRenderTarget.BindPS(0);
	//mScreenMeshBuffer.Render();
	//mDepthRenderTarget.UnbindPS(0);
}

// wvp -> WorldMatrix, ViewMatrix and ProjectionMatrix

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands