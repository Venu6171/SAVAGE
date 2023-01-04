#include "Precompiled.h"
#include "AnimationIO.h"

using namespace SAVAGE::Graphics;

void SAVAGE::Graphics::AnimationIO::Write(FILE* file, const AnimationClip& animClip)
{
	if (file == nullptr)
	{
		printf("Error saving a file");
		return;
	}
	fprintf_s(file, "BoneAnimationCount: %d\n", static_cast<uint32_t>(animClip.boneAnimations.size()));
	fprintf_s(file, "Name: %s\n", animClip.name.c_str(), animClip.name.size());
	fprintf_s(file, "Duration: %f\n", animClip.duration);
	fprintf_s(file, "TicksPerSecond: %f\n", animClip.ticksPerSecond);

	for (auto& boneAnim : animClip.boneAnimations)
	{
		if (boneAnim)
		{
			fprintf_s(file, "[Animation]\n");
			fprintf_s(file, "PositionKeyCount: %d\n", static_cast<uint32_t>(boneAnim->GetPositionKeys().size()));
			for (uint32_t i = 0; i < boneAnim->GetPositionKeys().size(); ++i)
			{
				fprintf_s(file, "PositionKey: %f %f %f\n",
					boneAnim->GetPositionKeys()[i].key.x,
					boneAnim->GetPositionKeys()[i].key.y,
					boneAnim->GetPositionKeys()[i].key.z);

				fprintf_s(file, "PositionKeyTime: %f\n",
					boneAnim->GetPositionKeys()[i].time);
			}

			fprintf_s(file, "RotationKeyCount: %d\n", static_cast<uint32_t>(boneAnim->GetRotationKeys().size()));

			for (uint32_t i = 0; i < boneAnim->GetRotationKeys().size(); ++i)
			{
				fprintf_s(file, "RotationKey: %f %f %f %f\n",
					boneAnim->GetRotationKeys()[i].key.w,
					boneAnim->GetRotationKeys()[i].key.x,
					boneAnim->GetRotationKeys()[i].key.y,
					boneAnim->GetRotationKeys()[i].key.z);

				fprintf_s(file, "RotationKeyTime: %f\n",
					boneAnim->GetRotationKeys()[i].time);
			}

			fprintf_s(file, "ScaleKeyCount: %d\n", static_cast<uint32_t>(boneAnim->GetScaleKeys().size()));

			for (uint32_t i = 0; i < boneAnim->GetScaleKeys().size(); ++i)
			{
				fprintf_s(file, "ScaleKeys: %f %f %f\n",
					boneAnim->GetScaleKeys()[i].key.x,
					boneAnim->GetScaleKeys()[i].key.y,
					boneAnim->GetScaleKeys()[i].key.z);

				fprintf_s(file, "ScaleKeyTime: %f\n",
					boneAnim->GetScaleKeys()[i].time);
			}
		}
		else
			fprintf_s(file, "[EMPTY]\n");
	}
}

void SAVAGE::Graphics::AnimationIO::Read(FILE* file, AnimationClip& animClip)
{
	if (file == nullptr)
	{
		printf("Error reading a file");
		return;
	}

	char buffer[256] = { "\0" };
	char nameBuffer[256] = { "\0" };

	char positionBuffer[256] = { "\0" };
	char rotationBuffer[256] = { "\0" };
	char scaleBuffer[256] = { "\0" };

	uint32_t boneAnimationCount = 0;
	fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &boneAnimationCount);
	animClip.boneAnimations.resize(boneAnimationCount);

	fscanf_s(file, "%s %s\n", buffer, (int)std::size(buffer), &nameBuffer, (int)std::size(nameBuffer));
	animClip.name = nameBuffer;

	fscanf_s(file, "%s %f\n", buffer, (int)std::size(buffer), &animClip.duration);
	fscanf_s(file, "%s %f\n", buffer, (int)std::size(buffer), &animClip.ticksPerSecond);

	for (uint32_t i = 0; i < boneAnimationCount; ++i)
	{
		fscanf_s(file, "%s\n", buffer, (int)std::size(buffer));
		if (strcmp(buffer, "[EMPTY]") == 0)
		{
			continue;
		}
		std::unique_ptr<Animation> boneAnimation = std::make_unique<Animation>();
		uint32_t positionKeyCount = 0;
		fscanf_s(file, "%s %d\n", positionBuffer, (int)std::size(positionBuffer), &positionKeyCount);
		boneAnimation->GetPositionKeys().resize(positionKeyCount);

		for (uint32_t j = 0; j < positionKeyCount; ++j)
		{
			fscanf_s(file, "%s %f %f %f\n", positionBuffer, (int)std::size(positionBuffer),
				&boneAnimation->GetPositionKeys()[j].key.x,
				&boneAnimation->GetPositionKeys()[j].key.y,
				&boneAnimation->GetPositionKeys()[j].key.z);

			fscanf_s(file, "%s %f", positionBuffer, (int)std::size(positionBuffer),
				&boneAnimation->GetPositionKeys()[j].time);
			boneAnimation->GetPositionKeys()[j].time;
		}

		uint32_t rotationKeyCount = 0;
		fscanf_s(file, "%s %d\n", rotationBuffer, (int)std::size(rotationBuffer), &rotationKeyCount);
		boneAnimation->GetRotationKeys().resize(rotationKeyCount);

		for (uint32_t j = 0; j < rotationKeyCount; ++j)
		{
			fscanf_s(file, "%s %f %f %f %f\n", rotationBuffer, (int)std::size(rotationBuffer),
				&boneAnimation->GetRotationKeys()[j].key.w,
				&boneAnimation->GetRotationKeys()[j].key.x,
				&boneAnimation->GetRotationKeys()[j].key.y,
				&boneAnimation->GetRotationKeys()[j].key.z);

			fscanf_s(file, "%s %f\n", rotationBuffer, (int)std::size(rotationBuffer),
				&boneAnimation->GetRotationKeys()[j].time);

			boneAnimation->GetRotationKeys()[j].time;
		}

		uint32_t scaleKeyCount = 0;
		fscanf_s(file, "%s %d\n", scaleBuffer, (int)std::size(scaleBuffer), &scaleKeyCount);
		boneAnimation->GetScaleKeys().resize(scaleKeyCount);

		for (uint32_t j = 0; j < scaleKeyCount; ++j)
		{
			fscanf_s(file, "%s %f %f %f\n", scaleBuffer, (int)std::size(scaleBuffer),
				&boneAnimation->GetScaleKeys()[j].key.x,
				&boneAnimation->GetScaleKeys()[j].key.y,
				&boneAnimation->GetScaleKeys()[j].key.z);

			fscanf_s(file, "%s %f\n", scaleBuffer, (int)std::size(scaleBuffer),
				&boneAnimation->GetScaleKeys()[j].time);

			boneAnimation->GetScaleKeys()[j].time;
		}

		animClip.boneAnimations[i] = std::move(boneAnimation);
	}
}