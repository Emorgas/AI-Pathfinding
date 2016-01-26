#include <assert.h>

#include "Entity.h"
#include "../ObjectTypes.h"
#include "../statemachine/State.h"
#include "../statemachine/FSMController.h"
#include "../statemachine/PlayerStates.h"
#include "../pathing/PathPlanner.h"
#include "../obstacle/ChargePack.h"
#include "../../gl/glut.h"

class Enemy;

class Player : public Entity
{
private:
	FSMController<Player>* _stateMachine;
	std::vector<ChargePack*> _packs;
	position _levelExit;

	float _xPos, _yPos, _zPos;
	float _redColor, _greenColor, _blueColor;
	float _size;

	position _currentPosition;
	position _offset;
	position _targetPosition;

	int _movementTimer;
	int _movementDelay = 250; //measured in milliseconds 500 = half a second

	Graph* _levelGraph;
	Path* _pathToFollow;

	//Range of the players weapon == to the number of charge packs collected
	int _weaponRange;
	int _weaponCooldownTimer;
	int _weaponRechargeTime = 1000;
	int _hitpoints;

	Enemy* _enemyInSight;
	bool _canISeeAnEnemy;
public:
	Player(int ID, Graph* levelGraph, float xPos, float yPos, float zPos, float red, float green, float blue, float size, std::vector<ChargePack*> packs);

	void Update(int deltaTime);

	void Render();

	//Control Methods
	void MoveOnPath();
	void CollectChargePack();
	void FireShotAtEnemy();
	void TakeHit();

	//Awareness Methods
	bool CheckForEnemyInSight();
	bool CanPlayerAttackAnEnemy();
	void ClearTargetEnemy();

	//Getters
	position GetPosition();
	position GetNearestObjective();
	position GetRandomObjective();
	Graph* GetLevelGraph() { return _levelGraph; }
	int GetWeaponRange() { return _weaponRange; }
	bool GetIsEnemyInSight() { return _canISeeAnEnemy; }
	Enemy* GetEnemyInSight() { return _enemyInSight; }
	FSMController<Player>* GetFSM() { return _stateMachine; }
	position GetTargetDestination() { return _targetPosition; }
	int GetWeaponCooldown() { return _weaponCooldownTimer; }
	int GetWeaponRechargeTime() { return _weaponRechargeTime; }
	position GetLevelExitPosition() { return _levelExit; }
	int GetMovementTimer() { return _movementTimer; }
	int GetMovementDelay() { return _movementDelay; }
	int GetHitpoints() { return _hitpoints; }
	Path* GetPathToFollow() { return _pathToFollow; }

	//Setters
	void SetPathToFollow(Path* path);
	void SetChargePackLocations(std::vector<ChargePack*> packs);
	void SetLevelExitLocation(position pos) { _levelExit = pos; }

	//Misc
	void ResetMovementTimer();
};