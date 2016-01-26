#include "Graph.h"
#include <ctime>

Graph::Graph(Obstacle** walls, int gridWidth, int gridHeight)
{
	_obstacles = walls;
	_gridWidth = gridWidth;
	_gridHeight = gridHeight;
	CreateNodes();
	CreateChildren();
	_numOfNodes = _nodes.size();
	srand(time(NULL));
}

void Graph::CreateNodes()
{
	position pos;
	bool occupied = false;
	for (int z = 1; z < _gridHeight + 1; z++)
	{
		pos.z = z;// +NUDGE;
		for (int x = 0; x < _gridWidth; x++)
		{
			occupied = false;
			pos.x = x;// +NUDGE;
			for (int i = 0; i < SCENARIO_WALL_QUANTITY; i++)
			{
				if (_obstacles[i]->GetPosition().x == pos.x && _obstacles[i]->GetPosition().z == pos.z)
				{
					occupied = true;
					_nodes.emplace_back(new Node(x, z, true, false));
					break;
				}
			}
			if (occupied != true)
				_nodes.emplace_back(new Node(x, z, false, false));
		}
	}
}

//Links all nodes to their children
void Graph::CreateChildren()
{
	for each (Node* var in _nodes)
	{
		//Travel Right
		Node* endPoint = NodeExistsAtPos((var->GetPosition().x + 1), var->GetPosition().z);
		if (endPoint != nullptr)
		{
			var->AddChildNode(endPoint);
		}
		//Travel Left
		endPoint = nullptr;
		endPoint = NodeExistsAtPos((var->GetPosition().x - 1), var->GetPosition().z);
		if (endPoint != nullptr)
		{
			var->AddChildNode(endPoint);
		}
		//Travel Down
		endPoint = nullptr;
		endPoint = NodeExistsAtPos((var->GetPosition().x), var->GetPosition().z + 1);
		if (endPoint != nullptr)
		{
			var->AddChildNode(endPoint);
		}
		//Travel Up
		endPoint = nullptr;
		endPoint = NodeExistsAtPos((var->GetPosition().x), var->GetPosition().z - 1);
		if (endPoint != nullptr)
		{
			var->AddChildNode(endPoint);
		}
	}
}

void Graph::ResetAllFAndGValues()
{
	for each (Node* var in _nodes)
	{
		var->SetG(0);
		var->SetH(0);
	}
}

void Graph::ResetAllDrawValues()
{
	for each (Node* var in _nodes)
	{
		var->SetDraw(false);
		var->SetColor(0.0f, 0.0f, 1.0f);
	}
}

Node* Graph::NodeExistsAtPos(float x, float z)
{
	for each (Node* var in _nodes)
	{
		if (var->GetPosition().x == x && var->GetPosition().z == z)
		{
			return var;
		}
	}
	return nullptr;
}

Node* Graph::GetRandomNode()
{	
	int index = rand() % _nodes.size();
	while (_nodes[index]->GetIsBlocked())
	{
		index = rand() % _nodes.size();
	}
	return _nodes[index];
}

Node* Graph::FindNearestUnblockedNodeTo(position pos)
{
	Node* node = NodeExistsAtPos(pos.x, pos.z);
	if (node != nullptr && node->GetIsBlocked() == false && node->GetHasEntityBool() == false)
	{
		return node;
	}
	else
	{
		int index = 0;
		while (node->GetIsBlocked() == true && node->GetHasEntityBool() == true)
		{
			switch (index)
			{
			case 0:
				node = NodeExistsAtPos(pos.x + 1, pos.z);
				break;
			case 1:
				node = NodeExistsAtPos(pos.x - 1, pos.z);
				break;
			case 2:
				node = NodeExistsAtPos(pos.x, pos.z + 1);
				break;
			case 3:
				node = NodeExistsAtPos(pos.x, pos.z - 1);
				break;
			case 4:
				node = FindNearestUnblockedNodeTo(GetRandomNode()->GetPosition());
				break;
			}
			index++;
		}

	}
}