#include <assert.h>

#include "Entity.h"
#include "../ObjectTypes.h"
#include "../statemachine/State.h"
#include "../statemachine/FSMController.h"
#include "../statemachine/EnemyStates.h"
#include "../pathing/PathPlanner.h"
#include "../../gl/glut.h"

class Enemy : public Entity
{
private:
	FSMController<Enemy>* _stateMachine;
	std::vector<position*> _objectives;

	float _xPos, _yPos, _zPos;
	float _redColor, _greenColor, _blueColor;
	float _size;

	position _currentPosition;
	position _offset;
	position _targetPosition;

	position _patrolStart;
	position _patrolEnd;
	int _positionInPatrolPath;
	bool _movingTowardsEnd;


	int _movementTimer;
	int _movementDelay = 250; //measured in milliseconds 500 = half a second

	Graph* _levelGraph;
	Path* _pathToFollow;
	Path* _patrolPath;

	//Range of the Enemys weapon == to the number of charge packs collected
	int _weaponRange;
	int _weaponCooldownTimer;
	int _weaponRechargeTime = 1000;
	int _hitpoints;

	Player* _enemyInSight;
	bool _canISeeAnEnemy;
public:
	Enemy(int ID, Graph* levelGraph, float xPos, float yPos, float zPos, float red, float green, float blue, float size, position patrolEnd);

	void Update(int deltaTime);

	void Render();

	//Control Methods
	void MoveOnPath();
	void MoveOnPatrolPath();
	void FireShotAtEnemy();
	void TakeHit();

	//Awareness Methods
	bool CheckForEnemyInSight();
	bool CanPlayerAttackAnEnemy();
	void ClearTargetEnemy();

	//Getters
	position GetPosition();
	Graph* GetLevelGraph() { return _levelGraph; }
	int GetWeaponRange() { return _weaponRange; }
	bool GetIsEnemyInSight() { return _canISeeAnEnemy; }
	Player* GetEnemyInSight() { return _enemyInSight; }
	FSMController<Enemy>* GetFSM() { return _stateMachine; }
	position GetTargetDestination() { return _targetPosition; }
	int GetWeaponCooldown() { return _weaponCooldownTimer; }
	int GetWeaponRechargeTime() { return _weaponRechargeTime; }
	int GetMovementTimer() { return _movementTimer; }
	int GetMovementDelay() { return _movementDelay; }
	int GetHitpoints() { return _hitpoints; }
	Path* GetPatrolPath() { return _patrolPath; }
	position GetPatrolPathStart() { return _patrolStart; }
	position GetCurrentPositionInPatrolPath() { return _patrolPath->path.at(_positionInPatrolPath)->GetPosition(); }
	position GetPatrolPathEnd() { return _patrolEnd; }
	Path* GetPathToFollow() { return _pathToFollow; }

	//Setters
	void SetPathToFollow(Path* path);
	void SetPatrolEndPosition(position pos) { _patrolEnd = pos; }
	void SetPatrolPath(Path* path) { _patrolPath = path; }

	//Misc
	void ResetMovementTimer();

};