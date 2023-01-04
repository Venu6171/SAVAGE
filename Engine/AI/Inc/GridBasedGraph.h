#pragma once

#include "Common.h"

namespace SAVAGE::AI
{
	class GridBasedGraph
	{
	public:
		enum Direction
		{
			North,
			South,
			East,
			West,
			NorthEast,
			NorthWest,
			SouthEast,
			SouthWest
		};

		struct Node
		{
			// Graph Structure - Set in Initialize
			std::array<Node*, 8> neighbors;
			int y = 0;
			int x = 0;

			// Search Params
			Node* parent = nullptr;
			float g = 0.0f;
			float h = 0.0f;
			bool opened = false;
			bool closed = false;
		};

		void Initialize(int columns, int rows);
		void ResetSearchParams();

		int GetNodeCount() const { return static_cast<int>(mNodes.size()); }

		//TODO: Add Bound checks here, if out of bound return nullptr
		Node* GetNode(int x, int y)
		{
			if (x < 0 || x >= mColumns ||
				y < 0 || y >= mRows)
			{
				return nullptr;
			}
			return &mNodes[x + (y * mColumns)];
		}
		const Node* GetNode(int x, int y) const
		{
			if (x < 0 || x >= mColumns ||
				y < 0 || y >= mRows)
			{
				return nullptr;
			}
			return &mNodes[x + (y * mColumns)];
		}

		int GetColumns() const { return mColumns; }
		int GetRows() const { return mRows; }

	private:
		std::vector<Node> mNodes;
		int mColumns = 0;
		int mRows = 0;
	};
}