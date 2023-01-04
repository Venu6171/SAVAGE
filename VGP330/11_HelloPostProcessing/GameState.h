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
	void RenderScene();
	void PostProcess();

	struct TransformData
	{
		SAVAGE::Math::Matrix4 world;
		SAVAGE::Math::Matrix4 wvp;
		SAVAGE::Math::Vector3 viewPosition;
		float padding = 0.0f;
	};

	struct TextureData
	{
		SAVAGE::Math::Matrix4 wvp;
	};

	SAVAGE::Graphics::Camera mCamera;
	
	SAVAGE::Graphics::RenderTarget mRenderTarget;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<TextureData> mTextureBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mMoonLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMoonMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	SAVAGE::Graphics::DirectionalLight mMoonDirectionalLight;
	SAVAGE::Graphics::Material mMoonMaterial;

	// TODO:
	// - Add VertexPN and MeshPN to graphics
	// - Add MeshBuilder::CreateSpherePN

	SAVAGE::Graphics::Mesh mMesh;
	SAVAGE::Graphics::MeshPX mCloudMesh;

	SAVAGE::Graphics::MeshBuffer mMeshBuffer;
	SAVAGE::Graphics::MeshBuffer mCloudMeshBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::MeshPX mSkyBoxMesh;
	SAVAGE::Graphics::MeshBuffer mSkyBoxMeshBuffer;

	SAVAGE::Graphics::VertexShader mPostProcessVertexShader;
	SAVAGE::Graphics::PixelShader mPostProcessPixelShader;

	SAVAGE::Graphics::VertexShader mTextureVertexShader;
	SAVAGE::Graphics::PixelShader  mTexturePixelShader;

	SAVAGE::Graphics::VertexShader mMoonVertexShader;
	SAVAGE::Graphics::PixelShader mMoonPixelShader;

	SAVAGE::Graphics::Sampler mSampler;
	SAVAGE::Graphics::BlendState mBlendState;

	SAVAGE::Graphics::Texture mTexture[5];
	SAVAGE::Graphics::Texture mCloudTexture;
	SAVAGE::Graphics::Texture mMoonTexture;
	SAVAGE::Graphics::Texture mSkyboxTexture;

	SAVAGE::Math::Vector3 mRotation;
	SAVAGE::Math::Vector3 mCloudRotation;
	SAVAGE::Math::Vector3 mMoonRotation;
	SAVAGE::Math::Vector3 mPosition;

	uint32_t rings = 0;
	uint32_t slices = 0;
	float radius = 0.0f;

	float mFPS = 0.0f;

	SAVAGE::Graphics::MeshPX mScreenMesh;
	SAVAGE::Graphics::MeshBuffer mScreenMeshBuffer;
};