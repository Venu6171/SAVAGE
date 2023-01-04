#pragma once

#include "AnimationClip.h"
#include "Material.h"
#include "MeshBuffer.h"
#include "MeshTypes.h"
#include "Skeleton.h"
#include "Texture.h"

namespace SAVAGE::Graphics
{
	class Model 
	{
	public:
		void Initialize(const std::filesystem::path& fileName);
		void AddAnimation(const std::filesystem::path& fileName);
		void Terminate();
		void Render() const;

		AnimationClip* GetAnimationClip(const std::string& name);

	public:
		struct MeshData
		{
			SkinnedMesh mesh;
			uint32_t materialIndex = 0;
			MeshBuffer meshBuffer;
		};

		struct MaterialData
		{
			Material material;
			std::string diffuseMapName;
			std::string specularMapName;
			std::string normalMapName;
			std::unique_ptr<Texture> diffuseTexture;
			std::unique_ptr<Texture> specularTexture;
			std::unique_ptr<Texture> normalTexture;	
		};

		std::vector<std::unique_ptr<MeshData>> meshData;
		std::vector<MaterialData> materialData;
		std::unique_ptr<Skeleton> skeleton;
		std::vector<std::unique_ptr<AnimationClip>> animSet;
	};
}