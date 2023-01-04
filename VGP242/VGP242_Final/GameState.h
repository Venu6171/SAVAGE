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
		SAVAGE::Math::Matrix4 wvp;

	};

	SAVAGE::Graphics::Camera mCamera;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;

	SAVAGE::Graphics::MeshPX mSkyboxMesh;
	SAVAGE::Graphics::MeshBuffer mSkyBoxMeshBuffer;

	SAVAGE::Graphics::MeshPX mMesh;
	SAVAGE::Graphics::MeshBuffer mMeshBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::Texture mTexture[9];
	SAVAGE::Graphics::Texture mSkyboxTexture;

	SAVAGE::Math::Vector3 mRotation;
	SAVAGE::Math::Vector3 mPosition;

	float mFPS= 0.0f;
};