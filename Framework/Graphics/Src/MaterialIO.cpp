#include "Precompiled.h"
#include "MaterialIO.h"

using namespace SAVAGE::Graphics;

void SAVAGE::Graphics::MaterialIO::Write(FILE* file, const Model::MaterialData& materialData)
{
	if (file == nullptr)
	{
		printf("Error saving a file");
		return;
	}

	fprintf_s(file, "Ambient: %f %f %f\n",
		materialData.material.ambient.r,
		materialData.material.ambient.g,
		materialData.material.ambient.b);

	fprintf_s(file, "Diffuse: %f %f %f\n",
		materialData.material.diffuse.r,
		materialData.material.diffuse.g,
		materialData.material.diffuse.b);

	fprintf_s(file, "Specular: %f %f %f\n",
		materialData.material.specular.r,
		materialData.material.specular.g,
		materialData.material.specular.b);

	fprintf_s(file, "Power: %f\n", materialData.material.power);

	if (materialData.diffuseMapName.empty())
	{
		fprintf_s(file, "DiffuseMapName: None\n");
	}
	else
	{
		fprintf_s(file, "DiffuseMapName: %s\n", materialData.diffuseMapName.c_str(), materialData.diffuseMapName.size());
	}

	if (materialData.specularMapName.empty())
	{
		fprintf_s(file, "SpecularMapName: None\n");
	}
	else
	{
		fprintf_s(file, "SpecularMapName: %s\n", materialData.specularMapName.c_str(), materialData.specularMapName.size());
	}

	if (materialData.normalMapName.empty())
	{
		fprintf_s(file, "NormalMapName: None\n");
	}
	else
	{
		fprintf_s(file, "NormalMapName: %s\n", materialData.normalMapName.c_str(), materialData.normalMapName.size());
	}
}

void SAVAGE::Graphics::MaterialIO::Read(FILE* file, Model::MaterialData& materialData)
{
	if (file == nullptr)
	{
		printf("Error reading a file");
		return;
	}

	char buffer[256] = { "\0" };
	fscanf_s(file, "%s %f %f %f", buffer, (int)std::size(buffer),
		&materialData.material.ambient.r,
		&materialData.material.ambient.g,
		&materialData.material.ambient.b);

	fscanf_s(file, "%s %f %f %f", buffer, (int)std::size(buffer),
		&materialData.material.diffuse.r,
		&materialData.material.diffuse.g,
		&materialData.material.diffuse.b);

	fscanf_s(file, "%s %f %f %f", buffer, (int)std::size(buffer),
		&materialData.material.specular.r,
		&materialData.material.specular.g,
		&materialData.material.specular.b);

	fscanf_s(file, "%s %f", buffer, (int)std::size(buffer), &materialData.material.power);

	char diffuseMapBuffer[256] = { "\0" };
	char specularMapBuffer[256] = { "\0" };
	char normalMapBuffer[256] = { "\0" };

	fscanf_s(file, "%s %s", buffer, (int)std::size(buffer), &diffuseMapBuffer, (int)std::size(diffuseMapBuffer));
	fscanf_s(file, "%s %s", buffer, (int)std::size(buffer), &specularMapBuffer, (int)std::size(specularMapBuffer));
	fscanf_s(file, "%s %s", buffer, (int)std::size(buffer), &normalMapBuffer, (int)std::size(normalMapBuffer));

	if (strcmp(diffuseMapBuffer, "None") != 0)
	{
		materialData.diffuseMapName = diffuseMapBuffer;
	}
	
	if (strcmp(specularMapBuffer, "None") != 0)
	{
		materialData.specularMapName = specularMapBuffer;
	}

	if (strcmp(normalMapBuffer, "None") != 0)
	{
		materialData.normalMapName = normalMapBuffer;
	}
}
