#pragma once
#include "Model.h"

using ModelId = int;
using AnimId = int;

namespace SAVAGE::Graphics
{
	class ModelManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();

	public:
		void Terminate();
		ModelId LoadModel(std::filesystem::path filePath, const std::vector<std::string>& animationPath);
		const Model* GetModel(ModelId id) const;

	private:
		struct Entry
		{
			std::filesystem::path filePath;
			std::unique_ptr<Model> model;
		};
		std::vector<Entry> mModels;
	};
}