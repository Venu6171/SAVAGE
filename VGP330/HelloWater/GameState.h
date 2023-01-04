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

	struct WaterData
	{
		SAVAGE::Math::Matrix4 world;
		SAVAGE::Math::Matrix4 wvp;
		SAVAGE::Math::Vector3 viewPosition;
		SAVAGE::Math::Matrix4 reflectionMatrix;
		float padding = 0.0f;
	};

	struct TextureData
	{
		SAVAGE::Math::Matrix4 wvp;
	};

	struct SettingsData
	{
		float waterTranslation = 1.0f;
		float reflectRefractScale = 1.0f;
		float padding[2];
	};

	SAVAGE::Graphics::Camera mDefaultCamera;

	SAVAGE::Graphics::TypedConstantBuffer<WaterData> mWaterBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<TextureData> mSkyBoxBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SettingsData> mSettingsBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	// TODO:
	// - Add VertexPN and MeshPN to graphics
	// - Add MeshBuilder::CreateSpherePN

	SAVAGE::Graphics::MeshPX mPlaneMesh;
	SAVAGE::Graphics::MeshPX mTextureMesh;

	SAVAGE::Graphics::ModelLoader mModelLoader;

	SAVAGE::Graphics::MeshBuffer mPlaneMeshBuffer;

	SAVAGE::Graphics::MeshPX mSkyBoxMesh;
	SAVAGE::Graphics::MeshBuffer mSkyBoxMeshBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::VertexShader mTextureVertexShader;
	SAVAGE::Graphics::PixelShader  mTexturePixelShader;

	SAVAGE::Graphics::Sampler mSampler;

	SAVAGE::Graphics::Texture mSkyboxTexture;
	SAVAGE::Graphics::Texture mPlaneNormalTexture;
	SAVAGE::Graphics::Texture mPlaneReflectionTexture;
	SAVAGE::Graphics::Texture mPlaneRefractionTexture;
 
	SAVAGE::Math::Vector3 mRotation;
	SAVAGE::Math::Vector3 mPosition = SAVAGE::Math::Vector3::Zero;

	float mFPS = 0.0f;

	SettingsData mSettingsData;
};