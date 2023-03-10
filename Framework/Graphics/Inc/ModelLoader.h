#pragma once

#include "MeshTypes.h"

namespace SAVAGE::Graphics
{
	class ModelLoader
	{
	public:
		static void LoadObjPX(std::filesystem::path fileName, float scale, MeshPX& mesh);
		static void LoadObj(std::filesystem::path fileName, float scale, Mesh& mesh);
		static void LoadModel(std::filesystem::path fileName, Model& model, bool binary = false);
		static void LoadMaterial(std::filesystem::path fileName, Model& model);
		static void LoadSkeleton(std::filesystem::path fileName, Model& model);
		static void LoadAnimationClip(std::filesystem::path fileName, Model& model);
	};
}