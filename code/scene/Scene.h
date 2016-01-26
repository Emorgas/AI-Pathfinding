#ifndef _SCENE_H_
#define _SCENE_H_

#include <windows.h>
#include <cstdio>
#include <GL\gl.h>
#include <GL\glu.h>
#include "../../gl/glut.h"
#include "../obstacle/Obstacle.h"
#include "../ObjectTypes.h"
#include "../node/NodeAndEdge.h"
#include "../node/Graph.h"
#include <vector>

using std::vector;

class Scene
{
private:
	Obstacle**	 m_pWalls;
	unsigned int m_iWallQty;
	position	 m_ScenarioOffset;

	Graph*		 m_graph;
	bool		 _searching = false;
public:
	Scene();
	~Scene();

	bool	Initialise();
	void	Render();
	void	Update(int a_deltaTime);								
	Graph* GetGraph() { return m_graph; }
private:
	void	DrawNodes();
	void	SetUpScenario();
	void	DrawScenario();
	void    UpdateScenario(int a_deltaTime);
	Node*   GetNodeAtPos(position pos);
};

#endif //_SCENE_H_
