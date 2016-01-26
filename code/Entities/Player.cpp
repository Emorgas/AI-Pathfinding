#include "Player.h"
#include "Enemy.h"
#include <ctime>

Player::Player(int ID, Graph* levelGraph, float xPos, float yPos, float zPos, float red, float green, float blue, float size, std::vector<ChargePack*> packs) :Entity(ID)
{
	_levelGraph = levelGraph;

	_xPos = xPos;// +NUDGE;
	_yPos = yPos;
	_zPos = zPos;// -NUDGE;

	_redColor = red;
	_greenColor = green;
	_blueColor = blue;

	_offset.x = 0.0f;
	_offset.z = 0.0f;

	_size = size;

	_currentPosition.x = xPos;// +NUDGE;
	_currentPosition.z = zPos;// -NUDGE;

	_movementTimer = 0;

	_levelGraph->NodeExistsAtPos(_currentPosition.x, _currentPosition.z)->EntityEnteredNode(this);

	_weaponRange = 1;
	_hitpoints = 4;
	_weaponCooldownTimer = 0;

	_canISeeAnEnemy = false;
	_enemyInSight = nullptr;

	_pathToFollow = nullptr;

	_packs = packs;
	_stateMachine = new FSMController<Player>(this);
	_stateMachine->ChangeState(CollectChargePack::Instance());
	_stateMachine->SetGlobalState(Fire::Instance());
}

void Player::Update(int deltaTime)
{
	if (_hitpoints > 0)
	{
		_redColor = 1.0f;
		_greenColor = 1.0f;
		_blueColor = 0.0f;
		if (_weaponCooldownTimer <= _weaponRechargeTime)
		{
			_weaponCooldownTimer += deltaTime;
		}
		_movementTimer += deltaTime;
		_stateMachine->Update();
	}
	else
	{
		_redColor = 1.0f;
		_greenColor = 1.0f;
		_blueColor = 1.0f;
	}
}

void Player::Render()
{
	glPushMatrix();
	glTranslatef(_currentPosition.x + _offset.x + NUDGE, _yPos, _currentPosition.z - _offset.z - NUDGE);
	glColor3f(_redColor, _greenColor, _blueColor);
	glutSolidCube(_size);
	glPopMatrix();
}

void Player::MoveOnPath()
{
	if (_pathToFollow->path.size() > 0)
	{
		_levelGraph->NodeExistsAtPos(_currentPosition.x, _currentPosition.z)->EntityLeftNode();
		Node* nextNode = _pathToFollow->path.at(0);
		position pos = nextNode->GetPosition();
		_xPos = pos.x;// +NUDGE;
		_currentPosition.x = pos.x;// +NUDGE;
		_zPos = pos.z;// -NUDGE;
		_currentPosition.z = pos.z;// -NUDGE;
		_pathToFollow->path.erase(_pathToFollow->path.begin());
		nextNode->EntityEnteredNode(this);
	}
}

void Player::CollectChargePack()
{
	for each (ChargePack* pack in _packs)
	{
		if (pack->GetPosition() == _currentPosition && pack->IsSpawned() == true)
		{
			pack->Collect();
			_weaponRange += 1;
		}
	}
}

void Player::FireShotAtEnemy()
{
	glColor3f(0.0f, 1.0f, 1.0f);
	glPointSize(2.0f);
	glBegin(GL_LINES);
	glVertex3f(_currentPosition.x, _yPos, _currentPosition.z);
	glVertex3f(_enemyInSight->GetPosition().x, _yPos, _enemyInSight->GetPosition().z);
	glEnd();
	_enemyInSight->TakeHit();
	_weaponCooldownTimer = 0;
	std::cout << "Shot Fired!" << std::endl;
	_redColor = 1.0f;
	_greenColor = 0.0f;
	_blueColor = 0.0f;
}

void Player::TakeHit()
{
	if (_hitpoints > 0)
	{
		_hitpoints -= 1;
	}
}

bool Player::CheckForEnemyInSight()
{
	Entity* tempEntity;
	Enemy* tempEnemy;
	int xPos;
	int zPos;
	//Looking Left
	for (int x = _currentPosition.x - 1; x >= _currentPosition.x - 6 && x >= 1; x--)
	{
		if (_levelGraph->NodeExistsAtPos(x, _currentPosition.z)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(x, _currentPosition.z)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}

	xPos = _currentPosition.x;
	zPos = _currentPosition.z;
	//Looking Up/Left Diagonal
	for (int i = 0; i < 2; i++)
	{
		if (xPos - 1 >= 0)//Check to make sure we are looking inside the map boundaries
			xPos -= 1;
		if (zPos - 1 >= 1)
			zPos -= 1;

		if (_levelGraph->NodeExistsAtPos(xPos, zPos)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(xPos, zPos)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}


	//Looking Right
	for (int x = _currentPosition.x + 1; x <= _currentPosition.x + 6 && x <= 20; x++)
	{
		if (_levelGraph->NodeExistsAtPos(x, _currentPosition.z)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(x, _currentPosition.z)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}

	xPos = _currentPosition.x;
	zPos = _currentPosition.z;
	//Looking Up/Right Diagonal
	for (int i = 0; i < 2; i++)
	{
		if (xPos + 1 <= 20)
			xPos += 1;
		if (zPos - 1 >= 1)
			zPos -= 1;
		if (_levelGraph->NodeExistsAtPos(xPos, zPos)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(xPos, zPos)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}

	//Looking Up
	for (int z = _currentPosition.z - 1; z >= _currentPosition.z - 6 && z >= 1; z--)
	{
		if (_levelGraph->NodeExistsAtPos(_currentPosition.x, z)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(_currentPosition.x, z)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}

	//Looking Down
	for (int z = _currentPosition.z + 1; z <= _currentPosition.z + 6 && z <= 20; z++)
	{
		if (_levelGraph->NodeExistsAtPos(_currentPosition.x, z)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(_currentPosition.x, z)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}

	xPos = _currentPosition.x;
	zPos = _currentPosition.z;
	//Looking Down/Left Diagonal
	for (int i = 0; i < 2; i++)
	{
		if (xPos - 1 >= 0)
			xPos -= 1;
		if (zPos + 1 <= 21)
			zPos += 1;
		if (_levelGraph->NodeExistsAtPos(xPos, zPos)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(xPos, zPos)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}

	xPos = _currentPosition.x;
	zPos = _currentPosition.z;
	//Looking Down/Right Diagonal
	for (int i = 0; i < 2; i++)
	{
		if (xPos + 1 <= 20)
			xPos += 1;
		if (zPos + 1 <= 21)
			zPos += 1;
		if (_levelGraph->NodeExistsAtPos(xPos, zPos)->GetIsBlocked() == false)
		{
			tempEntity = _levelGraph->NodeExistsAtPos(xPos, zPos)->GetEntityInNode();
			tempEnemy = (Enemy*)tempEntity;
			if (tempEntity != nullptr && tempEnemy->GetHitpoints() > 0)
			{
				_enemyInSight = (Enemy*)tempEntity;
				_canISeeAnEnemy = true;
				return true;
			}
		}
		else
			break;
	}


	_enemyInSight = nullptr;
	_canISeeAnEnemy = false;
	return false;
}

bool Player::CanPlayerAttackAnEnemy()
{
	if (_canISeeAnEnemy == true)
	{
		float d = (abs(_currentPosition.x - _enemyInSight->GetPosition().x) + abs(_currentPosition.z - _enemyInSight->GetPosition().z));
		if (d <= _weaponRange)
		{
			return true;
		}
		else
			return false;
	}
	return false;
}


void Player::ClearTargetEnemy()
{
	_enemyInSight = nullptr;
	_canISeeAnEnemy = false;
}

position Player::GetPosition()
{
	position tempPosition;

	tempPosition.x = _currentPosition.x + _offset.x;// -NUDGE;
	tempPosition.z = _currentPosition.z - _offset.z;// +NUDGE;

	return tempPosition;
}

position Player::GetNearestObjective()
{
	position pos;
	int d = INT_MAX;
	int dIndex = -1;
	int tempD = 0;

	for (unsigned int i = 0; i < _packs.size(); i++)
	{
		tempD = PathPlanner::GetEuclideanDistanceToPos(&_currentPosition, &_packs[i]->GetPosition());
		if (tempD < d && _packs[i]->IsSpawned() == true)
		{
			dIndex = i;
		}
	}
	if (dIndex != -1)
	{
		return _packs[dIndex]->GetPosition();
	}
	pos.x = -1;
	pos.z = -1;
	return pos;
}

position Player::GetRandomObjective()
{
	position pos;
	int d = INT_MAX;
	srand(time(NULL));
	int randIndex = rand() % _packs.size();

	while (_packs[randIndex]->IsSpawned() == false)
	{
		randIndex = rand() % _packs.size();
	}
	return _packs[randIndex]->GetPosition();
}

void Player::SetPathToFollow(Path* path)
{
	if (path != nullptr)
	{
		_pathToFollow = path;
		if (path->path.size() > 0)
		{
			_targetPosition = _pathToFollow->path.at(_pathToFollow->path.size() - 1)->GetPosition();
		}
		else
			_targetPosition = _currentPosition;
	}
}



void Player::SetChargePackLocations(std::vector<ChargePack*> packs)
{
	_packs = packs;
}

void Player::ResetMovementTimer()
{
	_movementTimer = 0;
}
