#include "Precompiled.h"
#include "AnimationBuilder.h"
#include "Model.h"
#include "ModelLoader.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;

void SAVAGE::Graphics::Model::Initialize(const std::filesystem::path& fileName)
{
	std::string modelFileName = fileName.string() + ".model";
	ModelLoader::LoadModel(modelFileName, *this);
	for (uint32_t i = 0; i < meshData.size(); ++i)
	{
		meshData[i]->meshBuffer.Initialize(meshData[i]->mesh);
	}

	std::string materialFileName = fileName.string() + ".material";
	ModelLoader::LoadMaterial(materialFileName, *this);

	std::filesystem::path fullFileName = materialFileName;
	std::filesystem::path filePath = fullFileName.parent_path();

	for (uint32_t i = 0; i < materialData.size(); ++i)
	{
		if (!materialData[i].diffuseMapName.empty())
		{
			std::filesystem::path texturePath = filePath / materialData[i].diffuseMapName;
			materialData[i].diffuseTexture = std::make_unique<Graphics::Texture>();
			materialData[i].diffuseTexture->Initialize(texturePath);
		}

		if (!materialData[i].specularMapName.empty())
		{
			std::filesystem::path texturePath = filePath / materialData[i].specularMapName;
			materialData[i].specularTexture = std::make_unique<Graphics::Texture>();
			materialData[i].specularTexture->Initialize(texturePath);
		}

		if (!materialData[i].normalMapName.empty())
		{
			std::filesystem::path texturePath = filePath / materialData[i].normalMapName;
			materialData[i].normalTexture = std::make_unique<Graphics::Texture>();
			materialData[i].normalTexture->Initialize(texturePath);
		}
	}

	std::string skeletonFileName = fileName.string() + ".skeleton";
	ModelLoader::LoadSkeleton(skeletonFileName, *this);
	for (auto& bone : skeleton->bones)
	{
		if (bone->parentIndex == -1)
		{
			skeleton->root = bone.get();
		}
		else
		{
			bone->parent = skeleton->bones[bone->parentIndex].get();
		}

		if (!bone->childIndices.empty())
		{
			for (int i = 0; i < bone->childIndices.size(); ++i)
			{
				bone->children.emplace_back(skeleton->bones[bone->childIndices[i]].get());
			}
		}
	}
}

void SAVAGE::Graphics::Model::AddAnimation(const std::filesystem::path& fileName)
{
	std::string animationFileName = fileName.string() + ".animset";
	ModelLoader::LoadAnimationClip(animationFileName, *this);
}

void SAVAGE::Graphics::Model::Terminate()
{
	for (uint32_t i = 0; i < meshData.size(); ++i)
	{
		meshData[i]->meshBuffer.Terminate();
	}

	for (uint32_t i = 0; i < materialData.size(); ++i)
	{
		if (materialData[i].diffuseTexture != nullptr)
		{
			materialData[i].diffuseTexture->Terminate();
		}
		if (materialData[i].specularTexture != nullptr)
		{
			materialData[i].specularTexture->Terminate();
		}
		if (materialData[i].normalTexture != nullptr)
		{
			materialData[i].normalTexture->Terminate();
		}
	}
}

void Model::Render() const
{
	for (uint32_t i = 0; i < meshData.size(); ++i)
	{
		auto& data = meshData[i];
		auto& material = materialData[data->materialIndex];
		if (material.diffuseTexture)
			material.diffuseTexture->BindPS(0);
		if (material.specularTexture)
			material.specularTexture->BindPS(1);
		if (material.normalTexture)
			material.normalTexture->BindPS(3);
		data->meshBuffer.Render();
	}
}

AnimationClip* SAVAGE::Graphics::Model::GetAnimationClip(const std::string& name)
{
	auto iter = std::find_if(animSet.begin(), animSet.end(), [&name](const std::unique_ptr<AnimationClip>& animationClip)
	{
		return name.compare(animationClip->name) == 0;
	});

	AnimationClip* clip = nullptr;
	if (iter != animSet.end())
	{
		clip = iter->get();
	}
	return clip;
}