#include "Precompiled.h"
#include "DFS.h"

using namespace SAVAGE::AI;

bool DFS::Search(GridBasedGraph& graph, int startX, int startY, int endX, int endY, std::function<bool(int, int)> isBlocked)
{
	graph.ResetSearchParams();

	auto node = graph.GetNode(startX, startY);
	mOpenList.push_back(node);
	node->opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		node = mOpenList.front(); // current
		mOpenList.pop_front();

		if (node == graph.GetNode(endX, endY)/*end*/)
		{
			found = true;
		}
		else
		{
			for (auto neighbour : node->neighbors)
			{
				if (neighbour != nullptr)
				{
					if (isBlocked(neighbour->x, neighbour->y))
					{
						continue;
					}
					if (!neighbour->closed && !neighbour->opened)
					{
						mOpenList.push_front(neighbour);
						neighbour->opened = true;
						neighbour->parent = node;
					}
				}
			}
		}
		mClosedList.push_back(node);
		node->closed = true;
	}
	return found;
}