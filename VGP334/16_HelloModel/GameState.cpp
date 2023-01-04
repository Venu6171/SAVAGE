#include "GameState.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);

	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.Pitch(0.5f);

	mPlaneMesh = MeshBuilder::CreatePlanePX(10, 15, 1.0f);
	mPlaneMeshBuffer.Initialize(mPlaneMesh);

	mPaladinModel.Initialize("../../Assets/Models/Paladin_Rumba/Paladin");
	mPaladinModel.AddAnimation("../../Assets/Models/Paladin_Rumba/Paladin");
	mPaladinModel.GetAnimationClip("Rumba")->mLooping = true;

	mAlienModel.Initialize("../../Assets/Models/Paladin_Samba/Paladin");
	mAlienModel.AddAnimation("../../Assets/Models/Paladin_Samba/Paladin");
	mAlienModel.GetAnimationClip("Samba")->mLooping = true;

	mVertexShader.Initialize(L"../../Assets/Shaders/Skinning.fx", BoneVertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Skinning.fx");

	mPlaneVertexShader.Initialize(L"../../Assets/Shaders/DoTexture.fx", VertexPX::Format);
	mPlanePixelShader.Initialize(L"../../Assets/Shaders/DoTexture.fx");

	mPlaneTexture.Initialize(L"../../Assets/Images/Ground.png");

	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mTextureBuffer.Initialize();

	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.ambient = SAVAGE::Graphics::Colors::White;
	mDirectionalLight.diffuse = SAVAGE::Graphics::Colors::White;
	mDirectionalLight.specular = SAVAGE::Graphics::Colors::Black;
	mDirectionalLight.direction = { 1.0f, -1.0f, 1.0f };

	mMaterial.ambient = SAVAGE::Graphics::Colors::White;
	mMaterial.diffuse = SAVAGE::Graphics::Colors::White;
	mMaterial.specular = SAVAGE::Graphics::Colors::Black;
}

void GameState::Terminate()
{
	mPlaneVertexShader.Terminate();
	mPlanePixelShader.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mPlaneTexture.Terminate();

	mPlaneMeshBuffer.Terminate();
	mAlienModel.Terminate();
	mPaladinModel.Terminate();

	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();

	mTextureBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

	if (playAnimation)
	{
		mAnimationTimer += deltaTime * mAnimationSpeed;
		mAlienAnimationTimer += deltaTime * mAnimationSpeed;
	}

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mPaladinModelRotation.y += deltaTime * 10.0f;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mPaladinModelRotation.y -= deltaTime * 10.0f;
	}
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mPaladinModelPosition.z -= deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mPaladinModelPosition.z += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::LCONTROL))
	{
		mPaladinModelRotation.z += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
	{
		mPaladinModelRotation.z -= deltaTime;
	}

	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matWorld = SAVAGE::Math::Matrix4::Identity();
	Matrix4 matWorldAlien = SAVAGE::Math::Matrix4::Identity();

	// Plane
	TextureData textureData;

	mTextureBuffer.BindVS(0);
	mTextureBuffer.BindPS(0);

	mPlaneTexture.BindPS(0);
	mPlaneTexture.BindVS(0);

	matWorld = Matrix4::Translation(mPlanePosition);
	textureData.wvp = SAVAGE::Math::Transpose(matWorld * matView * matProj);
	mTextureBuffer.Update(textureData);

	mPlaneVertexShader.Bind();
	mPlanePixelShader.Bind();

	mPlaneMeshBuffer.Render();

	// Model
	TransformData transformData;
	transformData.viewPosition = mCamera.GetPosition();

	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(mSettingsData);
	mSettingsBuffer.BindVS(3);

	matWorld =
		Matrix4::RotationQuaternion(Normalize(mPaladinModelRotation)) *
		Matrix4::Translation(mPaladinModelPosition);


	matWorldAlien =
		Matrix4::RotationQuaternion(Normalize(mAlienModelRotation)) *
		Matrix4::Translation(mAlienModelPosition);

	BoneTransformData boneData;

	std::vector<SAVAGE::Math::Matrix4> paladinLocalTransform; // Transform matrices that move each bone into the local space
	std::vector<SAVAGE::Math::Matrix4> alienLocalTransform;

	if (playAnimation)
	{
		GetAnimationTransform(*mPaladinModel.skeleton, *mPaladinModel.animSet[0], mAnimationTimer, paladinLocalTransform); // Use Animation Keyframe matrices		
		GetAnimationTransform(*mAlienModel.skeleton, *mAlienModel.animSet[0], mAlienAnimationTimer, alienLocalTransform); // Use Animation Keyframe matrices
	}
	else
	{
		GetToRootTransform(*mPaladinModel.skeleton, paladinLocalTransform); // Use bind pose matrices from the skeleton bone		
		GetToRootTransform(*mAlienModel.skeleton, alienLocalTransform); // Use bind pose matrices from the skeleton bone
	}

	if (toggleSkeleton)
	{
		for (size_t i = 0; i < mPaladinModel.skeleton->bones.size(); ++i)
		{
			paladinLocalTransform[i] = paladinLocalTransform[i] * matWorld;
		}
		DrawSkeleton(*mPaladinModel.skeleton, paladinLocalTransform);

		for (size_t i = 0; i < mAlienModel.skeleton->bones.size(); ++i)
		{
			alienLocalTransform[i] = alienLocalTransform[i] * matWorldAlien;
		}
		DrawSkeleton(*mAlienModel.skeleton, alienLocalTransform);
	}
	else
	{
		// Paladin with Shield&Sword
		transformData.world = SAVAGE::Math::Transpose(matWorld);
		transformData.wvp[0] = SAVAGE::Math::Transpose(matWorld * matView * matProj);
		mTransformBuffer.Update(transformData);

		for (size_t i = 0; i < mPaladinModel.skeleton->bones.size(); ++i)
		{
			auto& bone = mPaladinModel.skeleton->bones[i];
			boneData.boneTransforms[bone->index] =
				SAVAGE::Math::Transpose(bone->offsetTransform *
					paladinLocalTransform[bone->index]);
		}

		mBoneTransformBuffer.Update(boneData);
		mBoneTransformBuffer.BindVS(4);

		mVertexShader.Bind();
		mPixelShader.Bind();
		mPaladinModel.Render();

		// Paladin without Shield&Sword
		transformData.world = SAVAGE::Math::Transpose(matWorldAlien);
		transformData.wvp[0] = SAVAGE::Math::Transpose(matWorldAlien * matView * matProj);
		mTransformBuffer.Update(transformData);

		for (size_t i = 0; i < mAlienModel.skeleton->bones.size(); ++i)
		{
			auto& bone = mAlienModel.skeleton->bones[i];
			boneData.boneTransforms[bone->index] =
				SAVAGE::Math::Transpose(bone->offsetTransform *
					alienLocalTransform[bone->index]);
		}

		mBoneTransformBuffer.Update(boneData);
		mBoneTransformBuffer.BindVS(4);

		mVertexShader.Bind();
		mPixelShader.Bind();
		mAlienModel.Render();
	}

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	mMaterial.power = 20.0f;

	// Earth Light
	if (ImGui::DragFloat3("LightDirection", &mDirectionalLight.direction.x, 0.1f))
	{
		mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
	}
	//ImGui::DragFloat4("SpaceShipRotation", &mModelRotationQuaternion.x, 0.01f);

	ImGui::ColorEdit3("ModelLightAmbient", &mDirectionalLight.ambient.x);
	ImGui::ColorEdit3("ModelLightDiffuse", &mDirectionalLight.diffuse.x);
	ImGui::ColorEdit3("ModelLightSpecular", &mDirectionalLight.specular.x);

	// Earth Material
	ImGui::ColorEdit3("ModelMaterialAmbient", &mMaterial.ambient.x);
	ImGui::ColorEdit3("ModelMaterialDiffuse", &mMaterial.diffuse.x);
	ImGui::ColorEdit3("ModelMaterialSpecular", &mMaterial.specular.x);

	ImGui::SliderFloat("ModelMaterialPower", &mMaterial.power, 1.0f, 100.0f);

	ImGui::Checkbox("ToggleSkeleton", &toggleSkeleton);
	ImGui::Checkbox("PlayAnimation", &playAnimation);

	bool skinning = mSettingsData.useSkinning == 1;
	if (ImGui::Checkbox("UseSkinning", &skinning))
	{
		mSettingsData.useSkinning = skinning ? 1 : 0;
	}

	ImGui::DragFloat("AnimationSpeed", &mAnimationSpeed, 0.5f, 1.0f, 10.0f);

	ImGui::End();
}