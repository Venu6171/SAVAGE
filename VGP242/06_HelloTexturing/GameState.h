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

	SAVAGE::Graphics::MeshPX mMesh;
	SAVAGE::Graphics::MeshBuffer mVertexBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::Sampler mSampler[4];
	SAVAGE::Graphics::Texture mTexture[4];

	SAVAGE::Math::Vector3 mRotation;
	SAVAGE::Math::Vector3 mPosition;

	float mFPS= 0.0f;
};