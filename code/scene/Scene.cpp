#include "Scene.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include <string>

//--------------------------------------------------------------------------------------------------------

Scene::Scene()
{
	//Current scenario
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;

	return;
}

//--------------------------------------------------------------------------------------------------------

Scene::~Scene()
{
	//Delete the memory and references.

	//Scenario 
	if(m_pWalls != NULL)
	{
		for(unsigned int i=0; i < m_iWallQty; i++)
		{
			delete *(m_pWalls+i);
			*(m_pWalls+i) = NULL;
		}
		delete [] m_pWalls;
		m_pWalls = NULL;
	}

	return;
}

//--------------------------------------------------------------------------------------------------------

bool Scene::Initialise()
{
	glEnable(GL_DEPTH_TEST);

	SetUpScenario();
	m_graph = new Graph(m_pWalls, 21, 21);
	
	return true;
}

//--------------------------------------------------------------------------------------------------------

void Scene::Render()
{
	// Draw a gridded ground
    float length  = 21.0f;
    float spacing = 1.0f;
    float yPos	  = 0.0f;
    float iLine;
    //Change colour to yellow.
	glColor3f(0.6f,0.5f,0.2f);

    glBegin(GL_LINES);
       for(iLine = 0; iLine <= length; iLine += spacing)
       {
			//Draw forward lines
			glVertex3f(iLine, yPos, length);
			glVertex3f(iLine, yPos, 0);
    
			//Draw crossing lines
			glVertex3f(length, yPos, iLine);
			glVertex3f(0, yPos, iLine);
       }
    glEnd();
	DrawNodes();
	//Draw the Scenario.
	DrawScenario();
}

//--------------------------------------------------------------------------------------------------------

void Scene::Update(int a_deltaTime)
{
	//Update the Scenario.
	UpdateScenario(a_deltaTime);
}

//--------------------------------------------------------------------------------------------------------

void Scene::DrawScenario()
{
	//Walls
	for(unsigned int i = 0; i < m_iWallQty; i++)
		m_pWalls[i]->Render();

}

//Methods to set up pointer arrays to all the wall pieces.
void Scene::SetUpScenario()
{
	//Set amount of obstacles / walls for this scenario.
	m_iWallQty = SCENARIO_WALL_QUANTITY;

	//Set no offset, so drawing takes place in corner.
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;
			
	//Set up walls.
	m_pWalls = new Obstacle *[m_iWallQty];
	m_pWalls[0] = new Obstacle(0, 0.5, 20);
	m_pWalls[1] = new Obstacle(1, 0.5, 20);
	m_pWalls[2] = new Obstacle(2, 0.5, 20);
	m_pWalls[3] = new Obstacle(3, 0.5, 20);
	m_pWalls[4] = new Obstacle(4, 0.5, 20);
	m_pWalls[5] = new Obstacle(5, 0.5, 20);
	m_pWalls[6] = new Obstacle(6, 0.5, 20);
	m_pWalls[7] = new Obstacle(7, 0.5, 20);
	m_pWalls[8] = new Obstacle(8, 0.5, 20);
	m_pWalls[9] = new Obstacle(10, 0.5, 20);
	m_pWalls[10] = new Obstacle(11, 0.5, 20);
	m_pWalls[11] = new Obstacle(12, 0.5, 20);
	m_pWalls[12] = new Obstacle(13, 0.5, 20);
	m_pWalls[13] = new Obstacle(14, 0.5, 20);
	m_pWalls[14] = new Obstacle(15, 0.5, 20);
	m_pWalls[15] = new Obstacle(16, 0.5, 20);
	m_pWalls[16] = new Obstacle(17, 0.5, 20);
	m_pWalls[17] = new Obstacle(18, 0.5, 20);
	m_pWalls[18] = new Obstacle(19, 0.5, 20);
	m_pWalls[19] = new Obstacle(20, 0.5, 20);

	m_pWalls[20] = new Obstacle(0, 0.5, 19);
	m_pWalls[21] = new Obstacle(8, 0.5, 19);
	m_pWalls[22] = new Obstacle(10, 0.5, 19);
	m_pWalls[23] = new Obstacle(20, 0.5, 19);

	m_pWalls[24] = new Obstacle(0, 0.5, 18);
	m_pWalls[25] = new Obstacle(2, 0.5, 18);
	m_pWalls[26] = new Obstacle(3, 0.5, 18);
	m_pWalls[27] = new Obstacle(4, 0.5, 18);
	m_pWalls[28] = new Obstacle(6, 0.5, 18);
	m_pWalls[29] = new Obstacle(12, 0.5, 18);
	m_pWalls[30] = new Obstacle(13, 0.5, 18);
	m_pWalls[31] = new Obstacle(14, 0.5, 18);
	m_pWalls[32] = new Obstacle(16, 0.5, 18);
	m_pWalls[33] = new Obstacle(17, 0.5, 18);
	m_pWalls[34] = new Obstacle(18, 0.5, 18);
	m_pWalls[35] = new Obstacle(19, 0.5, 18);
	m_pWalls[36] = new Obstacle(20, 0.5, 18);

	m_pWalls[37] = new Obstacle(0, 0.5, 17);
	m_pWalls[38] = new Obstacle(1, 0.5, 17);
	m_pWalls[39] = new Obstacle(2, 0.5, 17);
	m_pWalls[40] = new Obstacle(4, 0.5, 17);
	m_pWalls[41] = new Obstacle(6, 0.5, 17);
	m_pWalls[42] = new Obstacle(7, 0.5, 17);
	m_pWalls[43] = new Obstacle(8, 0.5, 17);
	m_pWalls[44] = new Obstacle(9, 0.5, 17);
	m_pWalls[45] = new Obstacle(10, 0.5, 17);
	m_pWalls[46] = new Obstacle(11, 0.5, 17);
	m_pWalls[47] = new Obstacle(12, 0.5, 17);
	m_pWalls[48] = new Obstacle(14, 0.5, 17);
	m_pWalls[49] = new Obstacle(20, 0.5, 17);

	m_pWalls[50] = new Obstacle(0, 0.5, 16);
	m_pWalls[51] = new Obstacle(16, 0.5, 16);
	m_pWalls[52] = new Obstacle(17, 0.5, 16);
	m_pWalls[53] = new Obstacle(18, 0.5, 16);
	m_pWalls[54] = new Obstacle(20, 0.5, 16);

	m_pWalls[55] = new Obstacle(0, 0.5, 15);
	m_pWalls[56] = new Obstacle(1, 0.5, 15);
	m_pWalls[57] = new Obstacle(2, 0.5, 15);
	m_pWalls[58] = new Obstacle(3, 0.5, 15);
	m_pWalls[59] = new Obstacle(4, 0.5, 15);
	m_pWalls[60] = new Obstacle(5, 0.5, 15);
	m_pWalls[61] = new Obstacle(6, 0.5, 15);
	m_pWalls[62] = new Obstacle(7, 0.5, 15);
	m_pWalls[63] = new Obstacle(8, 0.5, 15);
	m_pWalls[64] = new Obstacle(10, 0.5, 15);
	m_pWalls[65] = new Obstacle(11, 0.5, 15);
	m_pWalls[66] = new Obstacle(12, 0.5, 15);
	m_pWalls[67] = new Obstacle(13, 0.5, 15);
	m_pWalls[68] = new Obstacle(14, 0.5, 15);
	m_pWalls[69] = new Obstacle(15, 0.5, 15);
	m_pWalls[70] = new Obstacle(16, 0.5, 15);
	m_pWalls[71] = new Obstacle(20, 0.5, 15);

	m_pWalls[72] = new Obstacle(0, 0.5, 14);
	m_pWalls[73] = new Obstacle(16, 0.5, 14);
	m_pWalls[74] = new Obstacle(18, 0.5, 14);
	m_pWalls[75] = new Obstacle(20, 0.5, 14);

	m_pWalls[76] = new Obstacle(0, 0.5, 13);
	m_pWalls[77] = new Obstacle(1, 0.5, 13);
	m_pWalls[78] = new Obstacle(3, 0.5, 13);
	m_pWalls[79] = new Obstacle(4, 0.5, 13);
	m_pWalls[80] = new Obstacle(5, 0.5, 13);
	m_pWalls[81] = new Obstacle(6, 0.5, 13);
	m_pWalls[82] = new Obstacle(7, 0.5, 13);
	m_pWalls[83] = new Obstacle(8, 0.5, 13);
	m_pWalls[84] = new Obstacle(9, 0.5, 13);
	m_pWalls[85] = new Obstacle(10, 0.5, 13);
	m_pWalls[86] = new Obstacle(11, 0.5, 13);
	m_pWalls[87] = new Obstacle(12, 0.5, 13);
	m_pWalls[88] = new Obstacle(13, 0.5, 13);
	m_pWalls[89] = new Obstacle(14, 0.5, 13);
	m_pWalls[90] = new Obstacle(15, 0.5, 13);
	m_pWalls[91] = new Obstacle(16, 0.5, 13);
	m_pWalls[92] = new Obstacle(18, 0.5, 13);
	m_pWalls[93] = new Obstacle(19, 0.5, 13);
	m_pWalls[94] = new Obstacle(20, 0.5, 13);

	m_pWalls[95] = new Obstacle(0, 0.5, 12);
	m_pWalls[96] = new Obstacle(20, 0.5, 12);

	m_pWalls[97] = new Obstacle(0, 0.5, 11);
	m_pWalls[98] = new Obstacle(2, 0.5, 11);
	m_pWalls[99] = new Obstacle(3, 0.5, 11);
	m_pWalls[100] = new Obstacle(5, 0.5, 11);
	m_pWalls[101] = new Obstacle(6, 0.5, 11);
	m_pWalls[102] = new Obstacle(8, 0.5, 11);
	m_pWalls[103] = new Obstacle(9, 0.5, 11);
	m_pWalls[104] = new Obstacle(10, 0.5, 11);
	m_pWalls[105] = new Obstacle(12, 0.5, 11);
	m_pWalls[106] = new Obstacle(13, 0.5, 11);
	m_pWalls[107] = new Obstacle(14, 0.5, 11);
	m_pWalls[108] = new Obstacle(15, 0.5, 11);
	m_pWalls[109] = new Obstacle(17, 0.5, 11);
	m_pWalls[110] = new Obstacle(18, 0.5, 11);
	m_pWalls[111] = new Obstacle(19, 0.5, 11);
	m_pWalls[112] = new Obstacle(20, 0.5, 11);

	m_pWalls[113] = new Obstacle(0, 0.5, 10);
	m_pWalls[114] = new Obstacle(2, 0.5, 10);
	m_pWalls[115] = new Obstacle(6, 0.5, 10);
	m_pWalls[116] = new Obstacle(10, 0.5, 10);
	m_pWalls[117] = new Obstacle(15, 0.5, 10);
	m_pWalls[118] = new Obstacle(20, 0.5, 10);

	m_pWalls[119] = new Obstacle(0, 0.5, 9);
	m_pWalls[120] = new Obstacle(1, 0.5, 9);
	m_pWalls[121] = new Obstacle(2, 0.5, 9);
	m_pWalls[122] = new Obstacle(4, 0.5, 9);
	m_pWalls[123] = new Obstacle(6, 0.5, 9);
	m_pWalls[124] = new Obstacle(8, 0.5, 9);
	m_pWalls[125] = new Obstacle(10, 0.5, 9);
	m_pWalls[126] = new Obstacle(11, 0.5, 9);
	m_pWalls[127] = new Obstacle(12, 0.5, 9);
	m_pWalls[128] = new Obstacle(13, 0.5, 9);
	m_pWalls[129] = new Obstacle(14, 0.5, 9);
	m_pWalls[130] = new Obstacle(15, 0.5, 9);
	m_pWalls[131] = new Obstacle(17, 0.5, 9);
	m_pWalls[132] = new Obstacle(18, 0.5, 9);
	m_pWalls[133] = new Obstacle(20, 0.5, 9);
	
	m_pWalls[134] = new Obstacle(0, 0.5, 8);
	m_pWalls[135] = new Obstacle(4, 0.5, 8);
	m_pWalls[136] = new Obstacle(8, 0.5, 8);
	m_pWalls[137] = new Obstacle(10, 0.5, 8);
	m_pWalls[138] = new Obstacle(17, 0.5, 8);
	m_pWalls[139] = new Obstacle(20, 0.5, 8);

	m_pWalls[140] = new Obstacle(0, 0.5, 7);
	m_pWalls[141] = new Obstacle(2, 0.5, 7);
	m_pWalls[142] = new Obstacle(4, 0.5, 7);
	m_pWalls[143] = new Obstacle(6, 0.5, 7);
	m_pWalls[144] = new Obstacle(8, 0.5, 7);
	m_pWalls[145] = new Obstacle(10, 0.5, 7);
	m_pWalls[146] = new Obstacle(12, 0.5, 7);
	m_pWalls[147] = new Obstacle(13, 0.5, 7);
	m_pWalls[148] = new Obstacle(14, 0.5, 7);
	m_pWalls[149] = new Obstacle(15, 0.5, 7);
	m_pWalls[150] = new Obstacle(17, 0.5, 7);
	m_pWalls[151] = new Obstacle(18, 0.5, 7);
	m_pWalls[152] = new Obstacle(20, 0.5, 7);

	m_pWalls[153] = new Obstacle(0, 0.5, 6);
	m_pWalls[154] = new Obstacle(2, 0.5, 6);
	m_pWalls[155] = new Obstacle(4, 0.5, 6);
	m_pWalls[156] = new Obstacle(6, 0.5, 6);
	m_pWalls[157] = new Obstacle(8, 0.5, 6);
	m_pWalls[158] = new Obstacle(10, 0.5, 6);
	m_pWalls[159] = new Obstacle(18, 0.5, 6);
	m_pWalls[160] = new Obstacle(20, 0.5, 6);

	m_pWalls[161] = new Obstacle(0, 0.5, 5);
	m_pWalls[162] = new Obstacle(2, 0.5, 5);
	m_pWalls[163] = new Obstacle(4, 0.5, 5);
	m_pWalls[164] = new Obstacle(5, 0.5, 5);
	m_pWalls[165] = new Obstacle(6, 0.5, 5);
	m_pWalls[166] = new Obstacle(8, 0.5, 5);
	m_pWalls[208] = new Obstacle(10, 0.5, 5);
	m_pWalls[167] = new Obstacle(13, 0.5, 5);
	m_pWalls[168] = new Obstacle(14, 0.5, 5);
	m_pWalls[169] = new Obstacle(15, 0.5, 5);
	m_pWalls[170] = new Obstacle(16, 0.5, 5);
	m_pWalls[171] = new Obstacle(17, 0.5, 5);
	m_pWalls[172] = new Obstacle(18, 0.5, 5);
	m_pWalls[173] = new Obstacle(20, 0.5, 5);

	m_pWalls[174] = new Obstacle(0, 0.5, 4);
	m_pWalls[175] = new Obstacle(2, 0.5, 4);
	m_pWalls[176] = new Obstacle(4, 0.5, 4);
	m_pWalls[177] = new Obstacle(6, 0.5, 4);
	m_pWalls[178] = new Obstacle(10, 0.5, 4);
	m_pWalls[179] = new Obstacle(11, 0.5, 4);
	m_pWalls[180] = new Obstacle(14, 0.5, 4);
	m_pWalls[181] = new Obstacle(16, 0.5, 4);
	m_pWalls[182] = new Obstacle(20, 0.5, 4);

	m_pWalls[183] = new Obstacle(0, 0.5, 3);
	m_pWalls[184] = new Obstacle(2, 0.5, 3);
	m_pWalls[185] = new Obstacle(8, 0.5, 3);
	m_pWalls[186] = new Obstacle(10, 0.5, 3);
	m_pWalls[187] = new Obstacle(20, 0.5, 3);

	m_pWalls[188] = new Obstacle(0, 0.5, 2);
	m_pWalls[189] = new Obstacle(1, 0.5, 2);
	m_pWalls[190] = new Obstacle(2, 0.5, 2);
	m_pWalls[191] = new Obstacle(3, 0.5, 2);
	m_pWalls[192] = new Obstacle(4, 0.5, 2);
	m_pWalls[193] = new Obstacle(5, 0.5, 2);
	m_pWalls[194] = new Obstacle(6, 0.5, 2);
	m_pWalls[195] = new Obstacle(7, 0.5, 2);
	m_pWalls[196] = new Obstacle(8, 0.5, 2);
	m_pWalls[197] = new Obstacle(10, 0.5, 2);
	m_pWalls[198] = new Obstacle(11, 0.5, 2);
	m_pWalls[199] = new Obstacle(12, 0.5, 2);
	m_pWalls[200] = new Obstacle(13, 0.5, 2);
	m_pWalls[201] = new Obstacle(14, 0.5, 2);
	m_pWalls[202] = new Obstacle(15, 0.5, 2);
	m_pWalls[203] = new Obstacle(16, 0.5, 2);
	m_pWalls[204] = new Obstacle(17, 0.5, 2);
	m_pWalls[205] = new Obstacle(18, 0.5, 2);
	m_pWalls[206] = new Obstacle(19, 0.5, 2);
	m_pWalls[207] = new Obstacle(20, 0.5, 2);


































	//Set all to color and offset.
	for(unsigned int i = 0; i < m_iWallQty; i++)
	{
		m_pWalls[i]->SetColor(1.0f, 0.0f, 0.0f);
		m_pWalls[i]->SetOffset(m_ScenarioOffset);
	}

	//Add your waypoint structure here.

	//Setup your edge list here.
}


void Scene::DrawNodes()
{
	std::vector<Node*> m_nodes;
	m_nodes = m_graph->GetNodes();
	for each (Node* var in m_nodes)
	{
		if (var->ToDraw())
		{
			glPushMatrix();
			glTranslatef(var->GetPosition().x + NUDGE, 0.5f, var->GetPosition().z - NUDGE);
			glColor3f(var->GetRed(), var->GetGreen(), var->GetBlue());
			glutSolidCube(0.25f);
			glPopMatrix();
		}
	}
}

//--------------------------------------------------------------------------------------------------------

void Scene::UpdateScenario(int a_deltaTime)
{
}

//--------------------------------------------------------------------------------------------------------