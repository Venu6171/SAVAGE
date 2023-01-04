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

	struct BoneTransformData
	{
		SAVAGE::Math::Matrix4 boneTransforms[256];
	};

	struct SettingsData
	{
		int useSpecularMap = 0;
		int useBumpMap = 0;
		int useNormalMap = 1;
		int useShadow = 0;
		int useSkinning = 0;
		float padding[3];
	};

	struct TextureData
	{
		SAVAGE::Math::Matrix4 wvp;
	};

	SAVAGE::Graphics::Camera mCamera;

	SAVAGE::Graphics::TypedConstantBuffer<TransformData> mTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<BoneTransformData> mBoneTransformBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SettingsData> mSettingsBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<TextureData> mTextureBuffer;

	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::DirectionalLight> mLightBuffer;
	SAVAGE::Graphics::TypedConstantBuffer<SAVAGE::Graphics::Material> mMaterialBuffer;

	// TODO: Show these in ImGui so we can change them on the fly
	SAVAGE::Graphics::DirectionalLight mDirectionalLight;
	SAVAGE::Graphics::Material mMaterial;

	SAVAGE::Graphics::Model mPaladinModel;
	SAVAGE::Graphics::Model mAlienModel;
	SAVAGE::Graphics::MeshPX mPlaneMesh;
	SAVAGE::Graphics::MeshBuffer mPlaneMeshBuffer;

	SAVAGE::Graphics::Texture mPlaneTexture;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;

	SAVAGE::Graphics::VertexShader mPlaneVertexShader;
	SAVAGE::Graphics::PixelShader  mPlanePixelShader;

	SAVAGE::Math::Vector3 mPlanePosition = { 2.0f, 0.0f, 0.0f };

	SAVAGE::Math::Vector3 mPaladinModelPosition = { 0.0f, 0.0f, 0.0f };
	SAVAGE::Math::Quaternion mPaladinModelRotation;

	SAVAGE::Math::Vector3 mAlienModelPosition = { 2.5f, 0.0f, 0.0f };
	SAVAGE::Math::Quaternion mAlienModelRotation;

	SAVAGE::AnimatorComponent mAnimator;

	float mFPS = 0.0f;
	float mAnimationTimer = 0.0f;
	float mAlienAnimationTimer = 0.0f;
	bool toggleSkeleton = false;
	bool playAnimation = false;
	float mAnimationSpeed = 1.0f;
	SettingsData mSettingsData;
};