#pragma once

#include "SAVAGE/Inc/SAVAGE.h"

class GameState : public SAVAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render();
	void DebugUI() override;

private:
	struct TransformData
	{
		SAVAGE::Math::Matrix4 world;
		SAVAGE::Math::Matrix4 wvp[2];
		SAVAGE::Math::Vector3 viewPosition;
		float padding;
	};

	struct GlassData
	{
		SAVAGE::Math::Matrix4 wvp;
	};

	struct GlitchData
	{
		SAVAGE::Math::Matrix4 wvp;
	};

	struct HologramData
	{
		SAVAGE::Math::Matrix4 world;
		SAVAGE::Math::Matrix4 wvp;
		SAVAGE::Math::Vector3 viewPosition;
		float padding;
	};

	struct GlitchSettingsData
	{
		float chromoAmountX = -0.10f;
		float chromoAmountY = 0.0f;
		float glitchEffect = 0.2f;
		float desaturationAmount = 1.0f;
	};

	struct GlassSettingsData
	{
		float refractionScale = -0.02f;
		float padding[3];
	};

	struct HologramSettingsData
	{
		float scale = 1.0f;
		float scrollSpeed = 1.0f;
		float glowIntensity = 1.0f;
		float glitchSpeed = 1.0f;
	};

	SAVAGE::Graphics::Camera mCamera;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<GlassData> mGlassBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<GlitchData> mGlitchDataBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<HologramData> mHologramDataBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<GlassSettingsData> mGlassSettingsBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<GlitchSettingsData> mGlitchSettingsBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<HologramSettingsData> mHologramSettingsBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	// TODO:
	// - Add VertexPN and MeshPN to graphics
	// - Add MeshBuilder::CreateSpherePN

	SAVAGE::Graphics::MeshPX mPlaneMesh;
	SAVAGE::Graphics::Mesh mModelMesh;
	SAVAGE::Graphics::MeshPX mGlassMesh;
	SAVAGE::Graphics::MeshPX mGlitchMesh;

	SAVAGE::Graphics::ModelLoader mModelLoader;

	SAVAGE::Graphics::MeshBuffer mPlaneMeshBuffer;
	SAVAGE::Graphics::MeshBuffer mModelMeshBuffer;
	SAVAGE::Graphics::MeshBuffer mGlassMeshBuffer;
	SAVAGE::Graphics::MeshBuffer mGlitchMeshBuffer;

	SAVAGE::Graphics::VertexShader mGlassVertexShader;
	SAVAGE::Graphics::PixelShader  mGlassPixelShader;

	SAVAGE::Graphics::VertexShader mPlaneTextureVertexShader;
	SAVAGE::Graphics::PixelShader  mPlaneTexturePixelShader;

	SAVAGE::Graphics::VertexShader mGlitchVertexShader;
	SAVAGE::Graphics::PixelShader  mGlitchPixelShader;

	SAVAGE::Graphics::VertexShader mHologramVertexShader;
	SAVAGE::Graphics::PixelShader  mHologramPixelShader;

	SAVAGE::Graphics::Sampler mSampler;
	SAVAGE::Graphics::BlendState mBlendState;

	SAVAGE::Graphics::Texture mPlaneTexture;

	SAVAGE::Graphics::Texture mTexture;
	SAVAGE::Graphics::Texture mNormalTexture;
	SAVAGE::Graphics::Texture mRefractionTexture;

	SAVAGE::Graphics::Texture mGlitchTexture;

	SAVAGE::Graphics::Texture mModelDiffuseTexture;
	SAVAGE::Graphics::Texture mAlphaTexture;

	SAVAGE::Math::Vector3 mPlaneRotation;
	SAVAGE::Math::Vector3 mPlanePosition = { 0.0f, 0.0f, 0.0f };

	SAVAGE::Math::Vector3 mModelRotation;
	SAVAGE::Math::Vector3 mModelPosition = { 0.0f, 1.5f, 0.0f };

	SAVAGE::Math::Vector3 mGlassRotation = { -1.5f, 0.0f, 0.0f };
	SAVAGE::Math::Vector3 mGlassPosition = { 0.0f, 1.5f, 0.0f };

	SAVAGE::Math::Vector3 mGlitchRotation;
	SAVAGE::Math::Vector3 mGlitchPosition = { 10.0f, 1.5f, 0.0f };

	float mFPS = 0.0f;

	GlitchSettingsData mGlitchSettingsData;
	GlassSettingsData mGlassSettingsData;
	HologramSettingsData mHologramSettingsData;
};