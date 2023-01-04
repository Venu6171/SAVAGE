#pragma once

#include "MeshTypes.h"

namespace SAVAGE::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPC CreateCubePC();
		static MeshPX CreateCubePX();
		static MeshPX CreatePlanePX(uint32_t rows, uint32_t columns, float spacing, float tiling = 1.0f);
		static MeshPX CreateCylinderPX(uint32_t rings, uint32_t slices, float height, float radius);
		static MeshPX CreateSpherePX(uint32_t rings, uint32_t slices, float radius);
		static MeshPX CreateSkySpherePX(uint32_t rings, uint32_t slices, float radius);
		static MeshPX CreateSkyBox(float size);
		static MeshPX CreateScreenMesh();

		static MeshPN CreateSpherePN(uint32_t rings, uint32_t slices, float radius);

		static MeshPCX CreateSpherePCX(uint32_t rings, uint32_t slices, float radius);

		static Mesh CreateSphere(uint32_t rings, uint32_t slices, float radius);
		static Mesh CreatePlane(uint32_t rows, uint32_t columns, float spacing, float tiling = 1.0f);
	};
}