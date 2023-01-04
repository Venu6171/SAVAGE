#pragma once
#include "Model.h"

namespace SAVAGE::Graphics::MaterialIO
{
	void Write(FILE* file, const Model::MaterialData& materialData);
	void Read(FILE* file, Model::MaterialData& materialData);
}