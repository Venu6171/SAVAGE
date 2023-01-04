#pragma once

#include "VertexTypes.h"

namespace SAVAGE::Graphics
{
	template <class VertexT>
	struct MeshBase
	{
		using VertexType = VertexT;
		std::vector<VertexT> vertices;
		std::vector<uint32_t> indices;
	};

	using MeshPC = MeshBase<VertexPC>;
	using MeshPX = MeshBase<VertexPX>;
	using MeshPN = MeshBase<VertexPN>;
	using MeshPCX = MeshBase<VertexPCX>;
	using Mesh = MeshBase<Vertex>;
	using SkinnedMesh = MeshBase<BoneVertex>;
}