#pragma once
#include <vector>
#include "../ObjectTypes.h"
#include "../Entities/Entity.h"
//class Node;
//class Edge
//{
//public:
//	Edge(Node* startNode, Node* endNode, int cost) : _startNode(startNode), _endNode(endNode), _cost(cost) { ; }
//	int GetCost() const { return _cost; }
//	Node* GetStartNode() const { return _startNode; }
//	Node* GetEndNode() const { return _endNode; }
//private:
//	Node* _startNode;
//	Node* _endNode;
//	int _cost;
//};

class Node
{
public:
	Node(float x, float z, bool isBlocked, bool isOccupied)
	{
		_pos.x = x;
		_pos.z = z;
		_G = 0;
		_H = 0;
		_isBlocked = isBlocked;
		_containsEntity = nullptr;
		_draw = false;
	};
	std::vector<Node*> GetChildNodes() const { return _childNodes; }
	void AddChildNode(Node* newNode) { _childNodes.push_back(newNode); }

	//Agent management
	Entity* GetEntityInNode() { return _containsEntity; }
	bool GetHasEntityBool() { 
		if (_containsEntity != nullptr)
			return true;
		else
			return false;
	}
	void EntityEnteredNode(Entity* e) { _containsEntity = e; }
	void EntityLeftNode() { _containsEntity = nullptr; }


	position GetPosition() const { return _pos; }
	float GetG() { return _G; }
	void SetG(float g) { _G = g; }
	float GetH() { return _H; }
	void SetH(float h) { _H = h; }
	bool GetIsBlocked() { return _isBlocked; }
	void SetIsBlocked(bool b) { _isBlocked = b; }
	void SetPathParent(Node* node) { _pathParent = node; }
	Node* GetPathParent() { return _pathParent; }
	void SetDraw(bool b) { _draw = b; }
	void SetColor(float r, float g, float b) { _red = r; _green = g; _blue = b; }
	float GetRed() { return _red; }
	float GetGreen() { return _green; }
	float GetBlue() { return _blue; }
	bool ToDraw() { return _draw; }
	bool isSameNode(Node* n)
	{
		if (n->GetPosition().x == _pos.x && n->GetPosition().z == _pos.z)
			return true;
		else
			return false;
	}
private:
	std::vector<Node*> _childNodes;
	Node* _pathParent;
	Entity* _containsEntity;
	float _G;
	float _H;
	position _pos;
	bool _isBlocked;
	bool _draw;
	float _red;
	float _green;
	float _blue;
};

