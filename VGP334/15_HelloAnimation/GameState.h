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

	struct ColorData
	{
		SAVAGE::Math::Matrix4 wvp;
	};

	SAVAGE::Graphics::Camera mCamera;
	SAVAGE::Graphics::Animation mAnimation;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<TextureData> mTextureBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<ColorData> mColorBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	SAVAGE::Graphics::DirectionalLight mMoonDirectionalLight;
	SAVAGE::Graphics::Material mMoonMaterial;

	SAVAGE::Graphics::MeshPC mCubeMesh;
	SAVAGE::Graphics::MeshBuffer mCubeMeshBuffer;

	SAVAGE::Graphics::Mesh mModelMesh;
	SAVAGE::Graphics::ModelLoader mModelLoader;
	SAVAGE::Graphics::MeshBuffer mModelMeshBuffer;

	SAVAGE::Graphics::MeshPX mSkyBoxMesh;
	SAVAGE::Graphics::MeshBuffer mSkyBoxMeshBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::VertexShader mTextureVertexShader;
	SAVAGE::Graphics::PixelShader  mTexturePixelShader;

	SAVAGE::Graphics::VertexShader mColorVertexShader;
	SAVAGE::Graphics::PixelShader  mColorPixelShader;

	SAVAGE::Graphics::Sampler mSampler;

	SAVAGE::Graphics::Texture mSkyboxTexture;

	SAVAGE::Graphics::Texture mModelDiffuseTexture;
	SAVAGE::Graphics::Texture mModelSpecularTexture;
	SAVAGE::Graphics::Texture mModelBumpTexture;

	uint32_t rings = 0;
	uint32_t slices = 0;
	float radius = 0.0f;

	float mFPS = 0.0f;
	float mAnimationTimer = 0.0f;
};