#pragma once

#include "SAVAGE/Inc/SAVAGE.h"

class GameState : public SAVAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render();

private:
	struct TransformData
	{
		SAVAGE::Math::Matrix4 wvp;

	};

	SAVAGE::Graphics::Camera mCamera;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;

	SAVAGE::Graphics::MeshPC mMesh;
	SAVAGE::Graphics::MeshBuffer mVertexBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Math::Vector3 mRotation;
	SAVAGE::Math::Vector3 mPosition;
};