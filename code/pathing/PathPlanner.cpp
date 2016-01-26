#include "PathPlanner.h"
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>

std::vector<Node*> PathPlanner::_openList;
std::vector<Node*> PathPlanner::_closedList;
Graph* PathPlanner::_graph;
Node* PathPlanner::n_goal;
Node* PathPlanner::n_start;


PathPlanner::PathPlanner()
{

};
////////////////Forward Path Planning////////////////
Path* PathPlanner::Search(Graph* graph, int xStart, int zStart, int xGoal, int zGoal, bool drawPath) //pass graph, goal pos, start node
{
	Node* current;
	_graph = graph;
	_graph->ResetAllFAndGValues();
	if (drawPath)
		_graph->ResetAllDrawValues();
	n_goal = new Node(xGoal, zGoal, false, false);
	position pos;
	pos.x = xStart;
	pos.z = zStart;
	n_start = GetNodeAtPos(pos);
	n_start->SetH(CalculateHeuristic(n_start));

	_openList.clear();
	_closedList.clear();
	_openList.emplace_back(n_start);
	current = FindBestFromOpenList();

	while (!_openList.empty())
	{
		//Find node with lowest F
		current = FindBestFromOpenList();
		if (drawPath)
			current->SetDraw(true);
		//if current node is equal to goal then break the while look
		if (current->isSameNode(n_goal))
		{
			break;
		}
		_closedList.emplace_back(current);
		_openList.erase(_openList.begin() + NodeInList(current, _openList));
		std::vector<Node*> currentChildren = current->GetChildNodes();
		//Loop through node children
		for each(Node* child in currentChildren)
		{
			if (child->GetIsBlocked() == true || (child->GetHasEntityBool() == true && !child->isSameNode(n_goal)))
				continue;
			//Set cost of child to be the cost of current node plus the cost of travelling from current node to child node
			float childCurrentCost = (current->GetG() + 1);
			//see if child exists in open list
			int foundOpenIndex = NodeInList(child, _openList);
			int foundClosedIndex = NodeInList(child, _closedList);
			if (foundOpenIndex != -1)
			{
				//if the child is alread in the open list and our current g value is better than existing update child in list
				if (_openList[foundOpenIndex]->GetG() > childCurrentCost)
				{
					_openList[foundOpenIndex]->SetG(childCurrentCost);
					_openList[foundOpenIndex]->SetPathParent(current);
				}
				foundOpenIndex = -1;
			}
			else if ((foundClosedIndex != -1))
			{
				//if the child is alread in the closed list and our current g value is better than existing update child in list
				if (_closedList[foundClosedIndex]->GetG() > childCurrentCost)
				{
					_closedList[foundClosedIndex]->SetG(childCurrentCost);
					_closedList[foundClosedIndex]->SetPathParent(current);
				}
			}
			else
			{
				_openList.emplace_back(child);
				child->SetH(CalculateHeuristic(child));
				child->SetG(childCurrentCost);
				child->SetPathParent(current);
			}

		}
	}

	Path* nodePath = new Path;

	if (current->isSameNode(n_goal))
	{
		Node* temp = current;
		string path = "";
		nodePath->path.emplace_back(temp);
		path += std::to_string(temp->GetPosition().x) + ", " + std::to_string(temp->GetPosition().z) + "\n";
		while (!temp->isSameNode(n_start))
		{
			if (drawPath)
				temp->SetColor(0.0f, 1.0f, 0.0f);
			nodePath->path.emplace_back(temp);
			path += std::to_string(temp->GetPathParent()->GetPosition().x) + ", " + std::to_string(temp->GetPathParent()->GetPosition().z) + "\n";
			temp = temp->GetPathParent();
		}
		//std::cout << path;
		std::reverse(nodePath->path.begin(), nodePath->path.end());

		return nodePath;
	}
	return nodePath;
}

Node* PathPlanner::FindBestFromOpenList()
{
	int bestIndex = 0;
	float bestFValue = 9999999.0f;
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i]->GetH() + _openList[i]->GetG() < bestFValue)
		{
			bestFValue = (_openList[i]->GetG() + _openList[i]->GetH());
			bestIndex = i;
		}
	}
	return _openList.at(bestIndex);
}

float PathPlanner::CalculateHeuristic(Node* current)
{
	//Manhattan Distance
	/*float dx = abs(current->GetPosition().x - n_goal->GetPosition().x);
	float dz = abs(current->GetPosition().z - n_goal->GetPosition().z);
	current->SetH(dx + dz);
	return (dx + dz);*/

	//Euclidean Distance
	float h = sqrt(pow((current->GetPosition().x - n_goal->GetPosition().x), 2) + pow((current->GetPosition().z - n_goal->GetPosition().z), 2));
	return h;
}
////////////////Reverse Path Planning////////////////
Path* PathPlanner::SearchForFleePath(Graph* graph, int xStart, int zStart, int xGoal, int zGoal, bool drawPath, int searchDepth) //pass graph, goal pos, start node
{
	int currentSearchDepth = 0;
	int maxSearchDepth = searchDepth;
	Node* current;
	_graph = graph;
	_graph->ResetAllFAndGValues();
	if (drawPath)
		_graph->ResetAllDrawValues();
	n_goal = new Node(xGoal, zGoal, false, false);
	position pos;
	pos.x = xStart;
	pos.z = zStart;
	n_start = GetNodeAtPos(pos);
	n_start->SetH(CalculateFleeingHeuristic(n_start)); //Here we invert the heuristic value so that the further away a node is the better it's score

	_openList.clear();
	_closedList.clear();
	_openList.emplace_back(n_start);
	current = FindWorstFromOpenList(); //Bit of a counter intuitive name, we are finding the node
									   //with the highest f value which usually is the worst but in this situation is desireable

	while (!_openList.empty())
	{
		//Find node with lowest F
		current = FindWorstFromOpenList();
		if (drawPath)
			current->SetDraw(true);
		//Instead of checking to see if we are at the goal we simply check for the search depth
		if (currentSearchDepth >= maxSearchDepth)
		{
			break;
		}
		_closedList.emplace_back(current);
		_openList.erase(_openList.begin() + NodeInList(current, _openList));
		std::vector<Node*> currentChildren = current->GetChildNodes();
		//Loop through node children
		for each(Node* child in currentChildren)
		{
			if (child->GetIsBlocked() == true || (child->GetHasEntityBool() == true))
				continue;
			//Set cost of child to be the cost of current node plus the cost of travelling from current node to child node
			float childCurrentCost = (current->GetG() - 1);
			//see if child exists in open list
			int foundOpenIndex = NodeInList(child, _openList);
			int foundClosedIndex = NodeInList(child, _closedList);
			if (foundOpenIndex != -1)
			{
				//if the child is alread in the open list and our current g value is better than existing update child in list
				if (_openList[foundOpenIndex]->GetG() < childCurrentCost)
				{
					_openList[foundOpenIndex]->SetG(childCurrentCost);
					_openList[foundOpenIndex]->SetPathParent(current);
				}
				foundOpenIndex = -1;
			}
			else if ((foundClosedIndex != -1))
			{
				//if the child is alread in the closed list and our current g value is better than existing update child in list
				if (_closedList[foundClosedIndex]->GetG() < childCurrentCost)
				{
					_closedList[foundClosedIndex]->SetG(childCurrentCost);
					_closedList[foundClosedIndex]->SetPathParent(current);
				}
			}
			else
			{
				_openList.emplace_back(child);
				child->SetH(CalculateFleeingHeuristic(child));
				child->SetG(childCurrentCost);
				child->SetPathParent(current);
			}

		}
		currentSearchDepth++;
	}

	Path* nodePath = new Path;
	//No need to check if we are at out goal here as we aim to travel away from the goal node
	Node* temp = current;
	string path = "";
	nodePath->path.emplace_back(temp);
	path += std::to_string(temp->GetPosition().x) + ", " + std::to_string(temp->GetPosition().z) + "\n";
	while (!temp->isSameNode(n_start))
	{
		if (drawPath)
			temp->SetColor(0.0f, 1.0f, 0.0f);
		nodePath->path.emplace_back(temp);
		path += std::to_string(temp->GetPathParent()->GetPosition().x) + ", " + std::to_string(temp->GetPathParent()->GetPosition().z) + "\n";
		temp = temp->GetPathParent();
	}
	//std::cout << path;
	std::reverse(nodePath->path.begin(), nodePath->path.end());

	return nodePath;
}

Node* PathPlanner::FindWorstFromOpenList()
{
	int bestIndex = 0;
	float bestFValue = -9999999.0f;
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i]->GetH() + _openList[i]->GetG() > bestFValue)
		{
			bestFValue = (_openList[i]->GetG() + _openList[i]->GetH());
			bestIndex = i;
		}
	}
	return _openList.at(bestIndex);
}

float PathPlanner::CalculateFleeingHeuristic(Node* current)
{
	//Euclidean Distance
	float h = sqrt(pow((current->GetPosition().x - n_goal->GetPosition().x), 2) + pow((current->GetPosition().z - n_goal->GetPosition().z), 2));
	return -h;
}



float PathPlanner::NodeInList(Node* child, std::vector<Node*> list)
{
	int index = 0;
	for each (Node* var in list)
	{
		if (child->isSameNode(var))
		{
			return index;
		}
		index++;
	}
	return -1;
}

Node* PathPlanner::GetNodeAtPos(position pos)
{
	for each (Node* var in _graph->GetNodes())
	{
		if (var->GetPosition().x == pos.x && var->GetPosition().z == pos.z)
		{
			return var;
		}
	}
}

int PathPlanner::GetDistanceToPos(Graph* graph, position* startPos, position* endPos, bool drawPath)
{
	Path* path = Search(graph, startPos->x, startPos->z, endPos->x, endPos->z, drawPath);
	return path->path.size();
}

int PathPlanner::GetDistanceToPos(Graph* graph, position* startPos, position* endPos, Path* path, bool drawPath)
{
	path = Search(graph, startPos->x, startPos->z, endPos->x, endPos->z, drawPath);
	return path->path.size();
}

float PathPlanner::GetEuclideanDistanceToPos(position* startPos, position* endPos)
{
	float d = sqrt(pow((startPos->x - endPos->x), 2) + pow((startPos->z - endPos->z), 2));
	return d;
}



