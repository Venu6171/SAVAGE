#pragma once

#include "ModelComponent.h"
#include "Service.h"

namespace SAVAGE
{
	class AnimatorComponent;
	class ModelComponent;
	class TransformComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);

		void SetToggleSkeleton(bool showSkeleton) { mToggleSkeleton = showSkeleton; }
		SAVAGE::Graphics::Camera& GetCamera() { return mDefaultCamera; }
	private:
		void RenderDepthMap();
		void RenderScene();
		void RenderBrightness();
		void ApplyBlur();
		void PostProcess();

		struct Entry
		{
			const AnimatorComponent* animatorComponent = nullptr;
			const ModelComponent* modelComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
		};

		struct TransformData
		{
			SAVAGE::Math::Matrix4 world;
			SAVAGE::Math::Matrix4 wvp[2];
			SAVAGE::Math::Vector3 viewPosition;
			float padding = 0.0f;
		};

		struct TextureData
		{
			SAVAGE::Math::Matrix4 wvp;
			int useSkinning = 1;
			float padding[3];
		};

		struct BoneTransformData
		{
			SAVAGE::Math::Matrix4 boneTransforms[256];
		};

		struct SettingsData
		{
			int useSpecularMap = 0;
			int useBumpMap = 0;
			int useNormalMap = 0;
			int useShadow = 1;
			int useSkinning = 1;
			float depthBias = 0.0f;
			float padding[2];
		};

		struct ShadowData
		{
			int useBumpMap = 0;
			float depthDistance = 1.5f;
			float padding[2];
		};

		std::vector<Entry> mRenderEntries;

		SAVAGE::Graphics::Camera mDefaultCamera;
		SAVAGE::Graphics::Camera mLightCamera;
		SAVAGE::Graphics::Camera* mActiveCamera = nullptr;

		SAVAGE::Graphics::RenderTarget mDepthRenderTarget;
		SAVAGE::Graphics::RenderTarget mBaseRenderTarget;
		SAVAGE::Graphics::RenderTarget mBloomRenderTarget;
		SAVAGE::Graphics::RenderTarget mBlurRenderTarget;

		SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
		SAVAGE::Graphics::TypedConstantBuffer<BoneTransformData> mBoneTransformBuffer;
		SAVAGE::Graphics::TypedConstantBuffer<ShadowData> mShadowBuffer;
		SAVAGE::Graphics::TypedConstantBuffer<SettingsData> mSettingsBuffer;
		SAVAGE::Graphics::TypedConstantBuffer<TextureData> mTextureBuffer;
		SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Math::Vector4> mBlurSettingsBuffer;

		SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
		SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

		// TODO: Show these in ImGui so we can change them on the fly
		SAVAGE::Graphics::DirectionalLight mDirectionalLight;
		SAVAGE::Graphics::Material mMaterial;

		SAVAGE::Graphics::Mesh mPlaneMesh;
		SAVAGE::Graphics::MeshPX mSunMesh;
		SAVAGE::Graphics::MeshPX mSkyBoxMesh;

		SAVAGE::Graphics::MeshBuffer mSkyBoxMeshBuffer;
		SAVAGE::Graphics::MeshBuffer mSunMeshBuffer;
		SAVAGE::Graphics::MeshBuffer mPlaneMeshBuffer;

		SAVAGE::Graphics::Texture mPlaneTexture;
		SAVAGE::Graphics::Texture mSunTexture;
		SAVAGE::Graphics::Texture mBlankTexture;
		SAVAGE::Graphics::Texture mSkyboxTexture;

		SAVAGE::Graphics::Sampler mSampler;

		SAVAGE::Graphics::VertexShader mVertexShader;
		SAVAGE::Graphics::PixelShader mPixelShader;

		SAVAGE::Graphics::VertexShader mDepthMapVertexShader;
		SAVAGE::Graphics::PixelShader  mDepthMapPixelShader;

		SAVAGE::Graphics::VertexShader mBoneVertexShader;
		SAVAGE::Graphics::PixelShader  mBonePixelShader;

		SAVAGE::Graphics::VertexShader mPlaneVertexShader;
		SAVAGE::Graphics::PixelShader  mPlanePixelShader;
		
		SAVAGE::Graphics::VertexShader mGaussianBlurVertexShader;
		SAVAGE::Graphics::PixelShader mGaussianBlurXPixelShader;
		SAVAGE::Graphics::PixelShader mGaussianBlurYPixelShader;

		SAVAGE::Graphics::VertexShader mBloomVertexShader;
		SAVAGE::Graphics::PixelShader  mBloomPixelShader;

		SAVAGE::Graphics::VertexShader mTextureVertexShader;
		SAVAGE::Graphics::PixelShader  mTexturePixelShader;

		SAVAGE::Math::Vector3 mSunPosition = { -150.0f, 185.0f, -300.0f };
		SAVAGE::Math::Vector3 mSunSize = { 15.0f, 15.0f, 15.0f };

		SettingsData mSettingsData;
		ShadowData mDepthDistance;

		std::unique_ptr<TransformData> mTransformData;
		std::unique_ptr<TextureData> mTextureData;
		std::unique_ptr<BoneTransformData> mBoneData;

		uint32_t rings = 0;
		uint32_t slices = 0;
		float radius = 0.0f;

		int mBloomIterations = 5;
		float mBlurSaturation = 0.218f;
		float mFOV = 0.8f;
		float mDistance = 75.0f;
		bool mToggleSkeleton = false;

		SAVAGE::Graphics::MeshPX mScreenMesh;
		SAVAGE::Graphics::MeshBuffer mScreenMeshBuffer;
	};
}