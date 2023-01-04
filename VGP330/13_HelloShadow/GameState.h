#pragma once

#include <SAVAGE/Inc/SAVAGE.h>

class GameState : public SAVAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render();
	void DebugUI() override;

private:
	void RenderDepthMap();
	void RenderScene();
	void RenderBrightness();
	void PostProcess();

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
	};

	struct SettingsData
	{
		int useSpecularMap = 0;
		int useBumpMap = 0;
		int useNormalMap = 0;
		int useShadow = 0;
	};

	SAVAGE::Graphics::Camera mDefaultCamera;
	SAVAGE::Graphics::Camera mLightCamera;
	SAVAGE::Graphics::Camera* mActiveCamera = nullptr;

	SAVAGE::Graphics::RenderTarget mDepthRenderTarget;		// <-- record depth from light source
	SAVAGE::Graphics::RenderTarget mBaseRenderTarget;		// <-- original scene think "screenshot"
	SAVAGE::Graphics::RenderTarget mBrightRenderTarget;		// <-- only the "bright" pixels

	SAVAGE::Graphics::TypedConstantBuffer<TextureData> mDepthMapTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<TextureData> mTextureBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SettingsData> mSettingsBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mMoonLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMoonMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	// TODO:
	// - Add VertexPN and MeshPN to graphics
	// - Add MeshBuilder::CreateSpherePN

	SAVAGE::Graphics::Mesh mPlaneMesh;
	SAVAGE::Graphics::Mesh mModelMesh;
	SAVAGE::Graphics::MeshPX mTextureMesh;
	SAVAGE::Graphics::Mesh mBallMesh;

	SAVAGE::Graphics::ModelLoader mModelLoader;

	SAVAGE::Graphics::MeshBuffer mPlaneMeshBuffer;
	SAVAGE::Graphics::MeshBuffer mTextureMeshBuffer;
	SAVAGE::Graphics::MeshBuffer mBallMeshBuffer;
	SAVAGE::Graphics::MeshBuffer mModelMeshBuffer;

	SAVAGE::Graphics::MeshPX mSkyBoxMesh;
	SAVAGE::Graphics::MeshBuffer mSkyBoxMeshBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::VertexShader mTextureVertexShader;
	SAVAGE::Graphics::PixelShader  mTexturePixelShader;

	SAVAGE::Graphics::VertexShader mDepthMapVertexShader;
	SAVAGE::Graphics::PixelShader  mDepthMapPixelShader;

	SAVAGE::Graphics::Sampler mSampler;

	SAVAGE::Graphics::Texture mSkyboxTexture;
	SAVAGE::Graphics::Texture mPlaneTexture;
	SAVAGE::Graphics::Texture mBallTexture;

	SAVAGE::Graphics::Texture mModelDiffuseTexture;
	SAVAGE::Graphics::Texture mModelSpecularTexture;
	SAVAGE::Graphics::Texture mModelBumpTexture;

	SAVAGE::Math::Vector3 mRotation;
	SAVAGE::Math::Vector3 mPosition;

	SAVAGE::Math::Vector3 mBallRotation;
	SAVAGE::Math::Vector3 mBallPosition = { -10.0f, 2.0f, 0.0f };

	SAVAGE::Math::Vector3 mModelRotation;
	SAVAGE::Math::Vector3 mModelPosition = { 5.0f, 2.0f, 0.0f };

	uint32_t rings = 0;
	uint32_t slices = 0;
	float radius = 0.0f;

	float mFPS = 0.0f;

	SAVAGE::Graphics::MeshPX mScreenMesh;
	SAVAGE::Graphics::MeshBuffer mScreenMeshBuffer;

	SettingsData mSettingsData;
};