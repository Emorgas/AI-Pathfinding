#include "Worldmanager.h"
#include "../../gl/glut.h"
#include "../ObjectTypes.h"
#include <iostream>
#include <cstdlib> 
#include <ctime> 

//--------------------------------------------------------------------------------------------------------

WorldManager::WorldManager()
{
	//Default Constructor.
}

//--------------------------------------------------------------------------------------------------------

WorldManager::~WorldManager()
{
	if(m_pScenario != NULL)
	{
		delete m_pScenario;
		m_pScenario = NULL;
	}

	//Camera will be deleted elswehere.
	m_pCamera = NULL;

}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Initialise()
{
	//Set time varialbes.
	m_iTimeThisFrame = 0;
	m_iTimeLastFrame = 0;
	
	//Set the initial camera type to perspective.
	m_eCameraType = ORTHO;
	
	//Initialise the scene, but if an error occurs, then exit.
	m_pScenario = new Scene();
	m_pScenario->Initialise();

	//Add Chargepacks
	m_pChargePacks.emplace_back(new ChargePack(1.0f, 0.5f, 10.0f, 0.0f, 1.0f, 1.0f, 0.75f));
	m_pChargePacks.emplace_back(new ChargePack(1.0f, 0.5f, 3.0f, 0.0f, 1.0f, 1.0f, 0.75f));
	m_pChargePacks.emplace_back(new ChargePack(19.0f, 0.5f, 3.0f, 0.0f, 1.0f, 1.0f, 0.75f));

	//Determine Level Exit
	m_levelExitLocation.x = 20;
	m_levelExitLocation.z = 21;

	//Add agent(s)
	m_pCharacter = new Player(0, m_pScenario->GetGraph(), 0, 0.5, 21, 1.0f, 1.0f, 0.0f, 1.0f, m_pChargePacks);
	m_pCharacter->SetLevelExitLocation(m_levelExitLocation);

	//Add Enemies
	position patrolEnd;
	patrolEnd.x = 19;
	patrolEnd.z = 3;
	m_pEnemies.emplace_back(new Enemy(1, m_pScenario->GetGraph(), 3.0f, 0.5f, 3.0f, 1.0f, 0.0f, 1.0f, 1.0f, patrolEnd));
	patrolEnd.x = 14;
	patrolEnd.z = 19;
	m_pEnemies.emplace_back(new Enemy(2, m_pScenario->GetGraph(), 1.0f, 0.5f, 18.0f, 1.0f, 0.0f, 1.0f, 1.0f, patrolEnd));

}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Exec_loop()
{
	//Get time.
	m_iTimeThisFrame = GetTickCount();
	//exectuion loop.
	Update();
	Render();
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Update()
{
	//Get the delta time.
	int deltaTime = (m_iTimeThisFrame - m_iTimeLastFrame);

	//Set time last frame to the current time.
	m_iTimeLastFrame = m_iTimeThisFrame;


	//Update the scene.
	m_pScenario->Update(deltaTime);

	m_pCharacter->Update(deltaTime);

	for (int i = 0; i < m_pEnemies.size(); i++)
	{
		m_pEnemies[i]->Update(deltaTime);
	}
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Render()
{
	//Draw the scene
	m_pScenario->Render();

	//render the camera.
	m_pCamera->Render();

	m_pCharacter->Render();

	for (int i = 0; i < m_pEnemies.size(); i++)
	{
		m_pEnemies[i]->Render();
	}

	for each (ChargePack* pack in m_pChargePacks)
	{
		if (pack->IsSpawned())
			pack->Render();
	}
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
		case 27: 
			exit(0); 
		break;

		case 'q': case 'Q': //Move forward
		break;

		case 'a': case 'A': //Move backward
		break;
	
		case 'z': case 'Z': //Move left
		break;
	
		case 'x': case 'X': //Move right
		break;
	
		case ' ': 
		break;
  }
  return;
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::SpecialKey(int key, int x, int y)
{
	//Find out which key was pressed and make the relevant changes.
	switch(key)
	{
		case GLUT_KEY_F1:
			//Rerun this scenario.
			Initialise();
		break;

		case GLUT_KEY_F2:
		break;

		case GLUT_KEY_F3:
		break;
	}

	return;
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Mouse(int button, int state, int x,int y)
{
	return;
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Idle()
{
	return;
}

//--------------------------------------------------------------------------------------------------------

bool WorldManager::CheckLineOfSight(position pos1, position pos2)
{
	int deltaX = abs(pos2.x - pos1.x);
	int deltaZ = abs(pos2.z - pos1.z);
	int x = pos1.x;
	int z = pos1.z;
	int xInc1, xInc2, zInc1, zInc2, den, num, numAdd, numPixels;

	if (pos2.x >= pos1.x)
	{
		xInc1 = 1;
		xInc2 = 1;
	}
	else
	{
		xInc1 = -1;
		xInc2 = -1;
	}
	if (pos2.z >= pos1.z)
	{
		zInc1 = 1;
		zInc2 = 1;
	}
	else
	{
		zInc1 = -1;
		zInc2 = -1;
	}
	if (deltaX >= deltaZ)
	{
		xInc1 = 0;
		zInc2 = 0;
		den = deltaX;
		num = deltaX / 2;
		numAdd = deltaZ;
		numPixels = deltaX;
	}
	else
	{
		xInc2 = 0;
		zInc1 = 0;
		den = deltaZ;
		num = deltaZ / 2;
		numAdd = deltaX;
		numPixels = deltaZ;
	}

	for (int curPixel = 0; curPixel <= numPixels; curPixel++)
	{
		if (m_pScenario->GetGraph()->NodeExistsAtPos(x, z)->GetIsBlocked() == true)
		{
			return false;
		}
		num += numAdd;
		if (num >= den)
		{
			num -= den;
			x += xInc1;
			z += zInc1;
		}
		x += xInc2;
		z += zInc2;
	}
	return true;
}