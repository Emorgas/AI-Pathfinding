#pragma once

#include <vector>
#include "NodeAndEdge.h"
#include "../obstacle/Obstacle.h"

class Graph
{
private:
	std::vector<Node*> _nodes;
	Obstacle** _obstacles;
	int _gridWidth, _gridHeight;
	int _numOfNodes;

	void CreateNodes();
	void CreateChildren();


public:
	Graph(Obstacle** walls, int gridWidth, int gridHeight);
	Node* Graph::NodeExistsAtPos(float x, float z);
	std::vector<Node*> GetNodes() { return _nodes; }
	int NumOfNodes() { return _numOfNodes; }
	Node* GetRandomNode();
	void ResetAllFAndGValues();
	void ResetAllDrawValues();
	Node* Graph::FindNearestUnblockedNodeTo(position pos);


};