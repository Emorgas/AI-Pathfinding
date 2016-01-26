#pragma once
#include "State.h"
#include <iostream>

class PatrolBetweenTwoPoints : public State<Enemy>
{
private:
	PatrolBetweenTwoPoints() {}
	PatrolBetweenTwoPoints(const PatrolBetweenTwoPoints&);
	PatrolBetweenTwoPoints& operator=(const PatrolBetweenTwoPoints&);

public:
	static PatrolBetweenTwoPoints* Instance();

	virtual void Enter(Enemy* enemy);
	virtual void Update(Enemy* enemy);
	virtual void Exit(Enemy* enemy);

};

class FleeFromPlayer : public State<Enemy>
{
private:
	FleeFromPlayer() {}
	FleeFromPlayer(const FleeFromPlayer&);
	FleeFromPlayer& operator=(const FleeFromPlayer&);

public:
	static FleeFromPlayer* Instance();

	virtual void Enter(Enemy* enemy);
	virtual void Update(Enemy* enemy);
	virtual void Exit(Enemy* enemy);

};

class ChasePlayer : public State<Enemy>
{
private:
	ChasePlayer() {}
	ChasePlayer(const ChasePlayer&);
	ChasePlayer& operator=(const ChasePlayer&);

public:
	static ChasePlayer* Instance();

	virtual void Enter(Enemy* enemy);
	virtual void Update(Enemy* enemy);
	virtual void Exit(Enemy* enemy);

};

class EnemyFire : public State<Enemy>
{
private:
	EnemyFire() {}
	EnemyFire(const EnemyFire&);
	EnemyFire& operator=(const EnemyFire&);

public:
	static EnemyFire* Instance();

	virtual void Enter(Enemy* enemy);
	virtual void Update(Enemy* enemy);
	virtual void Exit(Enemy* enemy);

};