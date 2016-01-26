#pragma once
#include "../node/NodeAndEdge.h"
#include "../node/Graph.h"
#include <queue>

class PathPlanner
{
private:
	static Graph*				_graph;
	static int					_searchDepth;
	static std::vector<Node*>	_openList;
	static std::vector<Node*>	_closedList;


	static Node* n_goal;
	static Node* n_start;

	static Node* FindBestFromOpenList();
	static Node* FindWorstFromOpenList();
	static float CalculateHeuristic(Node* current);
	static float CalculateFleeingHeuristic(Node* current);
	static float NodeInList(Node* child, std::vector<Node*>list);
public:
	PathPlanner();
	static Path* Search(Graph* graph, int xStart, int zStart, int xGoal, int zGoal, bool drawPath);
	static Path* SearchForFleePath(Graph* graph, int xStart, int zStart, int xGoal, int zGoal, bool drawPath, int searchDepth);
	static Node* GetNodeAtPos(position pos);
	static int GetDistanceToPos(Graph* graph, position* startPos, position* endPos, bool drawPath); //Returns just the length of the path
	static int GetDistanceToPos(Graph* graph, position* startPos, position* endPos, Path* path, bool drawPath); //Returns length of path and path itself
	static float GetEuclideanDistanceToPos(position* startPos, position* endPos);
};