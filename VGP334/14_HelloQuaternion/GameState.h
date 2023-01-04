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
		SAVAGE::Math::Matrix4 wvp;
		SAVAGE::Math::Vector3 viewPosition;
		float padding;
	};

	struct TextureData
	{
		SAVAGE::Math::Matrix4 wvp;
	};

	SAVAGE::Graphics::Camera mCamera;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<TextureData> mTextureBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	SAVAGE::Graphics::DirectionalLight mMoonDirectionalLight;
	SAVAGE::Graphics::Material mMoonMaterial;

	SAVAGE::Graphics::Mesh mModelMesh;

	SAVAGE::Graphics::ModelLoader mModelLoader;
	SAVAGE::Graphics::MeshBuffer mModelMeshBuffer;

	SAVAGE::Graphics::MeshPX mSkyBoxMesh;
	SAVAGE::Graphics::MeshBuffer mSkyBoxMeshBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::VertexShader mTextureVertexShader;
	SAVAGE::Graphics::PixelShader  mTexturePixelShader;

	SAVAGE::Graphics::Sampler mSampler;
	SAVAGE::Graphics::BlendState mBlendState;

	SAVAGE::Graphics::Texture mSkyboxTexture;

	SAVAGE::Graphics::Texture mModelDiffuseTexture;
	SAVAGE::Graphics::Texture mModelSpecularTexture;
	SAVAGE::Graphics::Texture mModelBumpTexture;

	SAVAGE::Math::Quaternion mModelRotationQuaternion = SAVAGE::Math::Quaternion::Identity;
	SAVAGE::Math::Quaternion mCameraRotation = SAVAGE::Math::Quaternion::Identity;

	SAVAGE::Math::Vector3 mModelRotation = {0.0f, 1.0f, 0.0f};
	SAVAGE::Math::Vector3 mModelPosition = { 0.0f, 0.0f, 0.0f };

	uint32_t rings = 0;
	uint32_t slices = 0;
	float radius = 0.0f;

	float mFPS = 0.0f;
};