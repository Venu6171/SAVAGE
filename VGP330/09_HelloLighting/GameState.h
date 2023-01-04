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

	SAVAGE::Graphics::Camera mCamera;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	// TODO:
	// - Add VertexPN and MeshPN to graphics
	// - Add MeshBuilder::CreateSpherePN
	
	SAVAGE::Graphics::MeshPN mMesh;
	SAVAGE::Graphics::MeshBuffer mMeshBuffer;

	SAVAGE::Graphics::VertexShader mVertexShader[3];
	SAVAGE::Graphics::PixelShader mPixelShader[3];

	SAVAGE::Math::Vector3 mRotation;
	SAVAGE::Math::Vector3 mPosition;

	uint32_t rings = 0;
	uint32_t slices = 0;
	float radius = 0.0f;

	float mFPS = 0.0f;
};