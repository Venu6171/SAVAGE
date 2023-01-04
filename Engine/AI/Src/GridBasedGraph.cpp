#include "Precompiled.h"
#include "GridBasedGraph.h"

using namespace SAVAGE::AI;

void GridBasedGraph::Initialize(int columns, int rows)
{
	mColumns = columns;
	mRows = rows;

	mNodes.resize(columns * rows);

	//Create nodes
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			auto node = GetNode(x, y);
			node->x = x;
			node->y = y;
		}
	}

	//Create Neighbour
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			auto node = GetNode(x, y);

			// Finding Left Neighbour
			if (node->x != 0)
			{
				int leftX = node->x - 1;
				int leftY = node->y;
				node->neighbors[Direction::West] = GetNode(leftX, leftY);
			}
			// Finding Top Neighbour
			if (node->y != 0)
			{
				int topX = node->x;
				int topY = node->y - 1;
				node->neighbors[Direction::North] = GetNode(topX, topY);
			}

			// Finding Bottom Neighbour
			if (node->y != rows - 1)
			{
				int rightX = node->x;
				int rightY = node->y + 1;
				node->neighbors[Direction::South] = GetNode(rightX, rightY);
			}

			// Finding Right Neighbor
			if (node->x != columns - 1)
			{
				int bottomX = node->x + 1;
				int bottomY = node->y;
				node->neighbors[Direction::East] = GetNode(bottomX, bottomY);
			}

			// Finding Top Left Neighbour
			if (node->x != 0 && node->y != 0)
			{
				int topLeftX = node->x - 1;
				int topLeftY = node->y - 1;
				node->neighbors[Direction::NorthWest] = GetNode(topLeftX, topLeftY);
			}

			// Finding Bottom Left Neighbour
			if (node->x != 0 && node->y != rows - 1)
			{
				int bottomLeftX = node->x - 1;
				int bottomLeftY = node->y + 1;
				node->neighbors[Direction::SouthWest] = GetNode(bottomLeftX, bottomLeftY);
			}

			// Finding Bottom Right Neighbour
			if (node->x != columns - 1 && node->y != rows - 1)
			{
				int bottomRightX = node->x + 1;
				int bottomRightY = node->y + 1;
				node->neighbors[Direction::SouthEast] = GetNode(bottomRightX, bottomRightY);
			}

			// Finding Top Right Neighbour
			if (node->x != columns - 1 && node->y != 0)
			{
				int topRightX = node->x + 1;
				int topRightY = node->y - 1;
				node->neighbors[Direction::NorthEast] = GetNode(topRightX, topRightY);
			}
		}
	}
}

void SAVAGE::AI::GridBasedGraph::ResetSearchParams()
{
	for (auto& node : mNodes)
	{
		node.parent = nullptr;
		node.g = 0.0f;
		node.h = 0.0f;
		node.opened = false;
		node.closed = false;
	}
}
