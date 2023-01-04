#include "Precompiled.h"
#include "RenderService.h"

#include "AnimatorComponent.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void RenderService::Initialize()
{
	GraphicsSystem::Get()->SetVSync(false);

	mSettingsData.depthBias = 0.0001f;
	mActiveCamera = &mDefaultCamera;

	rings = 200;
	slices = 200;
	radius = 1.0f;

	mPlaneMesh = MeshBuilder::CreatePlane(100, 100, 1.0f, 50.0f);
	mSunMesh = MeshBuilder::CreateSpherePX(rings, slices, radius);
	mSkyBoxMesh = MeshBuilder::CreateSkySpherePX(rings, slices, radius);

	mPlaneMeshBuffer.Initialize(mPlaneMesh);
	mSunMeshBuffer.Initialize(mSunMesh);
	mSkyBoxMeshBuffer.Initialize(mSkyBoxMesh);

	constexpr uint32_t depthMapSize = 4096;
	mDepthRenderTarget.Initialize(depthMapSize, depthMapSize, Texture::Format::RGBA_F32);

	mBaseRenderTarget.Initialize(
		GraphicsSystem::Get()->GetBackBufferWidth(),
		GraphicsSystem::Get()->GetBackBufferHeight()
	);

	mBloomRenderTarget.Initialize(
		GraphicsSystem::Get()->GetBackBufferWidth(),
		GraphicsSystem::Get()->GetBackBufferHeight()
	);

	mBlurRenderTarget.Initialize(
		GraphicsSystem::Get()->GetBackBufferWidth(),
		GraphicsSystem::Get()->GetBackBufferHeight()
	);

	mVertexShader.Initialize(L"../../Assets/Shaders/Skinning.fx", BoneVertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Skinning.fx");

	mDepthMapVertexShader.Initialize(L"../../Assets/Shaders/DepthMapWithBones.fx", BoneVertex::Format);
	mDepthMapPixelShader.Initialize(L"../../Assets/Shaders/DepthMapWithBones.fx");

	mBoneVertexShader.Initialize(L"../../Assets/Shaders/DoTextureWithBones.fx", BoneVertex::Format);
	mBonePixelShader.Initialize(L"../../Assets/Shaders/DoTextureWithBones.fx");

	mPlaneVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPlanePixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mGaussianBlurVertexShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", VertexPX::Format);
	mGaussianBlurXPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "BlurXPS");
	mGaussianBlurYPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "BlurYPS");

	mBloomVertexShader.Initialize(L"../../Assets/Shaders/Bloom.fx", VertexPX::Format);
	mBloomPixelShader.Initialize(L"../../Assets/Shaders/Bloom.fx");

	mTextureVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mTexturePixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");

	mPlaneTexture.Initialize(L"../../Assets/Images/Grid2.jpg");
	mSunTexture.Initialize("../../Assets/Images/Sun.png");
	mSkyboxTexture.Initialize("../../Assets/Images/SkyTexture.jpg");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mShadowBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mTextureBuffer.Initialize();
	mBlurSettingsBuffer.Initialize();

	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.ambient = SAVAGE::Graphics::Color({ 0.871f, 0.871f, 0.871f, 1.0f });
	mDirectionalLight.diffuse = SAVAGE::Graphics::Color({ 0.588f, 0.588f, 0.588f, 1.0f });
	mDirectionalLight.specular = SAVAGE::Graphics::Colors::White;
	mDirectionalLight.direction = { 1.0f, -1.0f, 1.0f };

	mMaterial.ambient = SAVAGE::Graphics::Colors::White;
	mMaterial.diffuse = SAVAGE::Graphics::Colors::DimGray;
	mMaterial.specular = SAVAGE::Graphics::Colors::Black;

	mScreenMesh = MeshBuilder::CreateScreenMesh();
	mScreenMeshBuffer.Initialize(mScreenMesh);
}

void RenderService::Terminate()
{
	mScreenMeshBuffer.Terminate();

	mSunTexture.Terminate();

	mBoneVertexShader.Terminate();
	mBonePixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapPixelShader.Terminate();

	mSampler.Terminate();

	mSkyboxTexture.Terminate();
	mSkyBoxMeshBuffer.Terminate();

	mTextureBuffer.Terminate();

	mGaussianBlurVertexShader.Terminate();
	mGaussianBlurXPixelShader.Terminate();
	mGaussianBlurYPixelShader.Terminate();

	mBloomVertexShader.Terminate();
	mBloomPixelShader.Terminate();

	mPlaneVertexShader.Terminate();
	mPlanePixelShader.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mTexturePixelShader.Terminate();
	mTextureVertexShader.Terminate();

	mSunTexture.Terminate();
	mPlaneTexture.Terminate();

	mPlaneMeshBuffer.Terminate();
	mSunMeshBuffer.Terminate();

	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();

	mTextureBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mShadowBuffer.Terminate();
	mBlurSettingsBuffer.Terminate();

	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
	mDepthRenderTarget.Terminate();
	mBlurRenderTarget.Terminate();
	mBloomRenderTarget.Terminate();
	mBaseRenderTarget.Terminate();
}

void RenderService::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	mLightCamera.SetDirection(-mSunPosition);
	SAVAGE::Math::Vector3 newCameraPosition = -mLightCamera.GetDirection() * mDistance;
	mLightCamera.SetFov(mFOV);
	mLightCamera.SetPosition(newCameraPosition);
}

void RenderService::Render()
{
	RenderDepthMap();
	RenderScene();
	RenderBrightness();
	ApplyBlur();
	PostProcess();
}

void RenderService::DebugUI()
{
	ImGui::Begin("RenderService", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mMaterial.power = 20.0f;

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useLightCamera = (mActiveCamera == &mLightCamera);
		if (ImGui::Checkbox("Use Light Camera", &useLightCamera))
		{
			mActiveCamera = useLightCamera ? &mLightCamera : &mDefaultCamera;
		}

		ImGui::DragFloat("FOV", &mFOV, 0.01f);
		ImGui::DragFloat("LightCameraDistance", &mDistance, 1.0f);
		ImGui::DragFloat("DepthDistance", &mDepthDistance.depthDistance, 0.01f, 1.0f, 2.0f);
	}

	ImGui::DragFloat3("SunPosition", &mSunPosition.x, 0.1f);
	ImGui::DragFloat3("SunSize", &mSunSize.x, 1.0f);
	ImGui::SliderFloat("DepthBias", &mSettingsData.depthBias, 0.0001f, 0.1f, "%.4f");
	ImGui::DragInt("Bloom Iterations", &mBloomIterations, 1, 0, 20);
	ImGui::DragFloat("Blur Saturation", &mBlurSaturation, 0.001f, 0.0f, 1.0f);

	if (ImGui::CollapsingHeader("Light Settings"))
	{
		ImGui::ColorEdit3("ModelLightAmbient", &mDirectionalLight.ambient.x);
		ImGui::ColorEdit3("ModelLightDiffuse", &mDirectionalLight.diffuse.x);
		ImGui::ColorEdit3("ModelLightSpecular", &mDirectionalLight.specular.x);
	}
	if (ImGui::CollapsingHeader("Material Settings"))
	{
		ImGui::ColorEdit3("ModelMaterialAmbient", &mMaterial.ambient.x);
		ImGui::ColorEdit3("ModelMaterialDiffuse", &mMaterial.diffuse.x);
		ImGui::ColorEdit3("ModelMaterialSpecular", &mMaterial.specular.x);
		ImGui::SliderFloat("ModelMaterialPower", &mMaterial.power, 1.0f, 100.0f);
	}

	ImGui::Image(mDepthRenderTarget.GetRawData(), { 200.0f, 200.0f });

	ImGui::End();
}

void SAVAGE::RenderService::Register(const ModelComponent* modelComponent)
{
	auto& gameObject = modelComponent->GetOnwer();
	Entry& entry = mRenderEntries.emplace_back();
	entry.animatorComponent = gameObject.GetComponent<AnimatorComponent>();
	entry.modelComponent = modelComponent;
	entry.transformComponent = gameObject.GetComponent<TransformComponent>();
}

void SAVAGE::RenderService::Unregister(const ModelComponent* modelComponent)
{
	auto match = [&](const auto& entry) {return (entry.modelComponent == modelComponent); };
	auto iter = std::find_if(mRenderEntries.begin(), mRenderEntries.end(), match);
	if (iter != mRenderEntries.end())
	{
		mRenderEntries.erase(iter);
	}
}

void SAVAGE::RenderService::RenderDepthMap()
{
	mDepthRenderTarget.BeginRender(Colors::Black);

	Matrix4 matView = mLightCamera.GetViewMatrix();
	Matrix4 matProj = mLightCamera.GetProjectionMatrix();

	mTextureData = std::make_unique<TextureData>();
	mTextureBuffer.BindVS(0);

	mShadowBuffer.Update(mDepthDistance);
	mShadowBuffer.BindPS(3);

	for (auto& entry : mRenderEntries)
	{
		Vector3 position = entry.transformComponent->GetPosition();
		Quaternion rotation = entry.transformComponent->GetRotation();
		Vector3 scale = entry.transformComponent->GetScale();

		Matrix4 matWorld =
			Matrix4::RotationQuaternion(Normalize(rotation)) *
			Matrix4::Translation(position);

		mTextureData->wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
		mTextureBuffer.Update(*mTextureData);

		auto modelId = entry.modelComponent->GetModelId();
		auto model = ModelManager::Get()->GetModel(modelId);
		auto& animator = entry.animatorComponent->GetAnimator();

		mBoneData = std::make_unique<BoneTransformData>();
		mBoneTransformBuffer.BindVS(4);

		const auto& toLocalTransforms = animator.GetToLocalTransforms();
		for (auto& bone : model->skeleton->bones)
		{
			mBoneData->boneTransforms[bone->index] = Transpose(toLocalTransforms[bone->index]);
		}
		mBoneTransformBuffer.Update(*mBoneData);

		mDepthMapVertexShader.Bind();
		mDepthMapPixelShader.Bind();
		model->Render();
	}
	mDepthRenderTarget.EndRender();
}

void SAVAGE::RenderService::RenderScene()
{
	mBaseRenderTarget.BeginRender();

	Matrix4 matView = mActiveCamera->GetViewMatrix();
	Matrix4 matProj = mActiveCamera->GetProjectionMatrix();
	Matrix4 matWorld = Matrix4::Identity();

	Matrix4 matViewLight = mLightCamera.GetViewMatrix();
	Matrix4 matProjLight = mLightCamera.GetProjectionMatrix();

	/*SkyBox*/
	mTextureData = std::make_unique<TextureData>();
	mTextureBuffer.BindVS(0);
	mSkyboxTexture.BindPS(0);

	matWorld =
		Matrix4::Scaling(500.0f, 500.0f, 500.0f);
	mTextureData->wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTextureBuffer.Update(*mTextureData);

	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();
	mSkyBoxMeshBuffer.Render();

	mTransformData = std::make_unique<TransformData>();
	mTransformBuffer.BindVS(0);
	mTransformData->viewPosition = mActiveCamera->GetPosition();

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(mSettingsData);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mDepthRenderTarget.BindPS(4);

	// Plane
	mPlaneTexture.BindPS(0);
	mSampler.BindPS(0);
	mTransformData->world = SAVAGE::Math::Transpose(Matrix4::Identity());
	mTransformData->wvp[0] = SAVAGE::Math::Transpose(matView * matProj);
	mTransformData->wvp[1] = SAVAGE::Math::Transpose(matViewLight * matProjLight);
	mTransformBuffer.Update(*mTransformData);

	mPlaneVertexShader.Bind();
	mPlanePixelShader.Bind();
	mPlaneMeshBuffer.Render();

	// Model
	for (auto& entry : mRenderEntries)
	{
		Vector3 position = entry.transformComponent->GetPosition();
		Quaternion rotation = entry.transformComponent->GetRotation();
		Vector3 scale = entry.transformComponent->GetScale();

		matWorld =
			Matrix4::RotationQuaternion(Normalize(rotation)) *
			Matrix4::Translation(position) * Matrix4::Scaling(scale);

		mTransformData->world = SAVAGE::Math::Transpose(matWorld);
		mTransformData->wvp[0] = SAVAGE::Math::Transpose(matWorld * matView * matProj);
		mTransformData->wvp[1] = SAVAGE::Math::Transpose(matWorld * matViewLight * matProjLight);
		mTransformBuffer.Update(*mTransformData);

		auto modelId = entry.modelComponent->GetModelId();
		auto model = ModelManager::Get()->GetModel(modelId);
		auto& animator = entry.animatorComponent->GetAnimator();

		if (mToggleSkeleton)
		{
			std::vector<Matrix4> transforms = animator.GetSkeletonTrasforms();
			for (size_t i = 0; i < model->skeleton->bones.size(); ++i)
			{
				transforms[i] = transforms[i] * matWorld;
			}
			DrawSkeleton(*model->skeleton, transforms);
		}
		else
		{
			mBoneData = std::make_unique<BoneTransformData>();
			const auto& toLocalTransforms = animator.GetToLocalTransforms();
			for (auto& bone : model->skeleton->bones)
			{
				mBoneData->boneTransforms[bone->index] = Transpose(toLocalTransforms[bone->index]);
			}
			mBoneTransformBuffer.Update(*mBoneData);
			mBoneTransformBuffer.BindVS(4);

			mVertexShader.Bind();
			mPixelShader.Bind();

			model->Render();
		}
	}

	mDepthRenderTarget.UnbindPS(4);
	SimpleDraw::Render(mDefaultCamera);
	mBaseRenderTarget.EndRender();
}

void SAVAGE::RenderService::RenderBrightness()
{
	mBloomRenderTarget.BeginRender();

	Matrix4 matView = mActiveCamera->GetViewMatrix();
	Matrix4 matProj = mActiveCamera->GetProjectionMatrix();

	mTextureData = std::make_unique<TextureData>();
	mTextureBuffer.BindVS(0);
	mSunTexture.BindPS(0);
	mSampler.BindPS(0);

	// Sun
	Matrix4 matWorld =
		Matrix4::Scaling(mSunSize) *
		Matrix4::Translation(mSunPosition);

	mTextureData->wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTextureBuffer.Update(*mTextureData);

	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();
	mSunMeshBuffer.Render();

	for (auto& entry : mRenderEntries)
	{
		Vector3 position = entry.transformComponent->GetPosition();
		Quaternion rotation = entry.transformComponent->GetRotation();

		Matrix4 matWorld =
			Matrix4::RotationQuaternion(Normalize(rotation)) *
			Matrix4::Translation(position);

		mTextureData->wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
		mTextureBuffer.Update(*mTextureData);

		auto modelId = entry.modelComponent->GetModelId();
		auto model = ModelManager::Get()->GetModel(modelId);
		auto& animator = entry.animatorComponent->GetAnimator();

		mBoneData = std::make_unique<BoneTransformData>();
		mBoneTransformBuffer.BindVS(4);
		const auto& toLocalTransforms = animator.GetToLocalTransforms();
		for (auto& bone : model->skeleton->bones)
		{
			mBoneData->boneTransforms[bone->index] = Transpose(toLocalTransforms[bone->index]);
		}
		mBoneTransformBuffer.Update(*mBoneData);

		mBlankTexture.BindPS(0);
		mBoneVertexShader.Bind();
		mBonePixelShader.Bind();

		for (auto& mesh : model->meshData)
		{
			mesh->meshBuffer.Render();
		}
	}

	mBloomRenderTarget.EndRender();
}

void SAVAGE::RenderService::ApplyBlur()
{
	auto graphicsSystem = GraphicsSystem::Get();
	Vector4 settings;
	settings.x = static_cast<float>(graphicsSystem->GetBackBufferWidth());
	settings.y = static_cast<float>(graphicsSystem->GetBackBufferHeight());
	settings.z = mBlurSaturation;
	mBlurSettingsBuffer.Update(settings);
	mBlurSettingsBuffer.BindPS(0);

	mGaussianBlurVertexShader.Bind();
	mSampler.BindPS(0);

	for (int i = 0; i < mBloomIterations; ++i)
	{
		// Horizontal blur
		mBlurRenderTarget.BeginRender();
		mBloomRenderTarget.BindPS(0);

		mGaussianBlurXPixelShader.Bind();
		mScreenMeshBuffer.Render();

		Texture::UnbindPS(0);
		mBlurRenderTarget.EndRender();

		// Vertical blur
		mBloomRenderTarget.BeginRender();
		mBlurRenderTarget.BindPS(0);

		mGaussianBlurYPixelShader.Bind();
		mScreenMeshBuffer.Render();

		Texture::UnbindPS(0);
		mBloomRenderTarget.EndRender();
	}
}

void SAVAGE::RenderService::PostProcess()
{
	mBloomVertexShader.Bind();
	mBloomPixelShader.Bind();

	mSampler.BindPS(0);

	mBaseRenderTarget.BindPS(0);
	mBloomRenderTarget.BindPS(1);

	mScreenMeshBuffer.Render();

	Texture::UnbindPS(0);
	Texture::UnbindPS(1);

	mBaseRenderTarget.UnbindPS(0);
	mBloomRenderTarget.UnbindPS(1);
}