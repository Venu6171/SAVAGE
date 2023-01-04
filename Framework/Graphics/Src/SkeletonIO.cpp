#include "Precompiled.h"
#include "SkeletonIO.h"

void SAVAGE::Graphics::SkeletonIO::Write(FILE* file, const Skeleton& skeleton)
{
	if (file == nullptr)
	{
		printf("Error saving a file");
		return;
	}

	fprintf_s(file, "BonesCount: %d\n", skeleton.bones.size());

	for (auto& bone : skeleton.bones)
	{
		fprintf_s(file, "Name: %s\n", bone->name.c_str(), bone->name.size());

		const int index = bone->index;
		fprintf_s(file, "Index: %d\n", index);

		const int parentIndex = bone->parentIndex;
		fprintf_s(file, "ParentIndex: %d\n", parentIndex);

		const int childIndicesCount = bone->childIndices.size();
		fprintf_s(file, "ChildIndicesCount: %d\n", childIndicesCount);
		for (int i = 0; i < childIndicesCount; ++i)
		{
			fprintf_s(file, "ChildIndices: %d\n", bone->childIndices[i]);
		}

		fprintf_s(file, "ParentTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			bone->toParentTransform._11,
			bone->toParentTransform._12,
			bone->toParentTransform._13,
			bone->toParentTransform._14,
			bone->toParentTransform._21,
			bone->toParentTransform._22,
			bone->toParentTransform._23,
			bone->toParentTransform._24,
			bone->toParentTransform._31,
			bone->toParentTransform._32,
			bone->toParentTransform._33,
			bone->toParentTransform._34,
			bone->toParentTransform._41,
			bone->toParentTransform._42,
			bone->toParentTransform._43,
			bone->toParentTransform._44);

		fprintf_s(file, "OffsetTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			bone->offsetTransform._11,
			bone->offsetTransform._12,
			bone->offsetTransform._13,
			bone->offsetTransform._14,
			bone->offsetTransform._21,
			bone->offsetTransform._22,
			bone->offsetTransform._23,
			bone->offsetTransform._24,
			bone->offsetTransform._31,
			bone->offsetTransform._32,
			bone->offsetTransform._33,
			bone->offsetTransform._34,
			bone->offsetTransform._41,
			bone->offsetTransform._42,
			bone->offsetTransform._43,
			bone->offsetTransform._44);
	}
}

void SAVAGE::Graphics::SkeletonIO::Read(FILE* file, Bone& bone)
{
	if (file == nullptr)
	{
		printf("Error reading a file");
		return;
	}

	char buffer[256] = { "\0" };
	char nameBuffer[256] = { "\0" };

	fscanf_s(file, "%s %s\n", buffer, (int)std::size(buffer), &nameBuffer, (int)std::size(nameBuffer));

	bone.name = nameBuffer;

	fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &bone.index);

	fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &bone.parentIndex);

	int childIndicesCount = 0;
	fscanf_s(file, "%s %d", buffer, (int)std::size(buffer), &childIndicesCount);
	bone.childIndices.resize(childIndicesCount);
	for (int i = 0; i < childIndicesCount; ++i)
	{
		fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &bone.childIndices[i]);
	}

	fscanf_s(file, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", buffer, (int)std::size(buffer),
		&bone.toParentTransform._11,
		&bone.toParentTransform._12,
		&bone.toParentTransform._13,
		&bone.toParentTransform._14,
		&bone.toParentTransform._21,
		&bone.toParentTransform._22,
		&bone.toParentTransform._23,
		&bone.toParentTransform._24,
		&bone.toParentTransform._31,
		&bone.toParentTransform._32,
		&bone.toParentTransform._33,
		&bone.toParentTransform._34,
		&bone.toParentTransform._41,
		&bone.toParentTransform._42,
		&bone.toParentTransform._43,
		&bone.toParentTransform._44);

	fscanf_s(file, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", buffer, (int)std::size(buffer),
		&bone.offsetTransform._11,
		&bone.offsetTransform._12,
		&bone.offsetTransform._13,
		&bone.offsetTransform._14,
		&bone.offsetTransform._21,
		&bone.offsetTransform._22,
		&bone.offsetTransform._23,
		&bone.offsetTransform._24,
		&bone.offsetTransform._31,
		&bone.offsetTransform._32,
		&bone.offsetTransform._33,
		&bone.offsetTransform._34,
		&bone.offsetTransform._41,
		&bone.offsetTransform._42,
		&bone.offsetTransform._43,
		&bone.offsetTransform._44);
}
