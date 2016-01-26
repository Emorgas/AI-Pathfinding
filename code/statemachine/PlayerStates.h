#pragma once
#include "State.h"
#include <iostream>

class CollectChargePack : public State<Player>
{
private:
	CollectChargePack() {}
	CollectChargePack(const CollectChargePack&);
	CollectChargePack& operator=(const CollectChargePack&);

public:
	static CollectChargePack* Instance();

	virtual void Enter(Player* player);
	virtual void Update(Player* player);
	virtual void Exit(Player* player);

};

class FleeFromEnemy : public State<Player>
{
private:
	FleeFromEnemy() {}
	FleeFromEnemy(const FleeFromEnemy&);
	FleeFromEnemy& operator=(const FleeFromEnemy&);

public:
	static FleeFromEnemy* Instance();

	virtual void Enter(Player* player);
	virtual void Update(Player* player);
	virtual void Exit(Player* player);

};

class ChaseEnemy : public State<Player>
{
private:
	ChaseEnemy() {}
	ChaseEnemy(const ChaseEnemy&);
	ChaseEnemy& operator=(const ChaseEnemy&);

public:
	static ChaseEnemy* Instance();

	virtual void Enter(Player* player);
	virtual void Update(Player* player);
	virtual void Exit(Player* player);

};

class ExitLevel : public State<Player>
{
private:
	ExitLevel() {}
	ExitLevel(const ExitLevel&);
	ExitLevel& operator=(const ExitLevel&);

public:
	static ExitLevel* Instance();

	virtual void Enter(Player* player);
	virtual void Update(Player* player);
	virtual void Exit(Player* player);

};

class Fire : public State<Player>
{
private:
	Fire() {}
	Fire(const Fire&);
	Fire& operator=(const Fire&);

public:
	static Fire* Instance();

	virtual void Enter(Player* player);
	virtual void Update(Player* player);
	virtual void Exit(Player* player);

};