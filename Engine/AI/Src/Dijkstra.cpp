#include "Precompiled.h"
#include "Dijkstra.h"

using namespace SAVAGE::AI;

bool Dijkstra::Search(GridBasedGraph& graph, int startX, int startY, int endX, int endY, std::function<bool(int, int)> isBlocked, std::function<float(int, int, int, int)> getGCost)
{
	graph.ResetSearchParams();

	auto node = graph.GetNode(startX, startY);
	mOpenList.push_back(node);
	node->opened = true;
	node->g = 0.0f;

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
			for (auto& neighbour : node->neighbors)
			{
				if (neighbour != nullptr)
				{
					if (isBlocked(neighbour->x, neighbour->y))
					{
						continue;
					}

					if (!neighbour->opened)
					{
						neighbour->g = node->g + getGCost(node->x, node->y, neighbour->x, neighbour->y);
						neighbour->parent = node;

						auto iter = mOpenList.begin();
						for (; iter != mOpenList.end(); ++iter)
						{
							if ((*iter)->g > neighbour->g)
							{
								break;
							}
						}
						mOpenList.insert(iter, neighbour);
						neighbour->opened = true;

					}
					else if (!neighbour->closed)
					{
						float newG = node->g + getGCost(node->x, node->y, neighbour->x, neighbour->y);

						if (newG < neighbour->g)
						{
							neighbour->g = newG;
							neighbour->parent = node;
							mOpenList.sort([](GridBasedGraph::Node* node1, GridBasedGraph::Node* node2)
							{
								return node1->g < node2->g;
							});
						}
					}
				}
			}
		}
		mClosedList.push_back(node);
		node->closed = true;
	}
	return found;
}