#include "EnemyStates.h"
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"
//////COLLECT CHARGE PACK//////
PatrolBetweenTwoPoints* PatrolBetweenTwoPoints::Instance()
{
	static PatrolBetweenTwoPoints instance;

	return &instance;
}
void PatrolBetweenTwoPoints::Enter(Enemy* enemy)
{

	if (enemy->GetPatrolPath() == nullptr)
	{
		std::cout << "Another Day... Time to start my patrol." << std::endl;
		//Start the patrol path at spawn position, select a random node at least x spaces away as the end
		//enemy->SetPatrolEndPosition(enemy->GetLevelGraph()->GetRandomNode()->GetPosition());
		enemy->SetPatrolPath(PathPlanner::Search(enemy->GetLevelGraph(), enemy->GetPatrolPathStart().x, enemy->GetPatrolPathStart().z,
			enemy->GetPatrolPathEnd().x, enemy->GetPatrolPathEnd().z, false));
	}
	else
	{
		std::cout << "Back to my patrol..." << std::endl;
		//We calculate a path back to the last patrol position and resume the patrol from that point
		enemy->SetPathToFollow(PathPlanner::Search(enemy->GetLevelGraph(), enemy->GetPosition().x, enemy->GetPosition().z,
			enemy->GetCurrentPositionInPatrolPath().x, enemy->GetCurrentPositionInPatrolPath().z, false));
	}
}

void PatrolBetweenTwoPoints::Update(Enemy* enemy)
{
	enemy->CheckForEnemyInSight();

	//if enemy is in line of sight && range is < enemy range run away
	if (enemy->GetIsEnemyInSight() == true && enemy->GetWeaponRange() < enemy->GetEnemyInSight()->GetWeaponRange())
	{
		//Run away from enemy
		std::cout << "He's too strong! Run Away!" << std::endl;
		enemy->GetFSM()->ChangeState(FleeFromPlayer::Instance());
	}
	//if enemy is in line of sight && range is > enemy range engage
	else if (enemy->GetIsEnemyInSight() == true && enemy->GetWeaponRange() > enemy->GetEnemyInSight()->GetWeaponRange())
	{
		//Chase the enemy down
		std::cout << "Enemy Sighted! Get Him!" << std::endl;
		enemy->GetFSM()->ChangeState(ChasePlayer::Instance());
	}

	//Move towards pack
	if (enemy->GetMovementTimer() > enemy->GetMovementDelay())
	{
		enemy->MoveOnPatrolPath();
		enemy->ResetMovementTimer();
	}
}

void PatrolBetweenTwoPoints::Exit(Enemy* enemy)
{
	//Debug text
}

//////FLEE FROM ENEMY//////
FleeFromPlayer* FleeFromPlayer::Instance()
{
	static FleeFromPlayer instance;

	return &instance;
}
void FleeFromPlayer::Enter(Enemy* enemy)
{
	enemy->SetPathToFollow(PathPlanner::SearchForFleePath(enemy->GetLevelGraph(), enemy->GetPosition().x,
		enemy->GetPosition().z, enemy->GetEnemyInSight()->GetPosition().x,
		enemy->GetEnemyInSight()->GetPosition().z, false, 20));
}

void FleeFromPlayer::Update(Enemy* enemy)
{
	enemy->CheckForEnemyInSight();

	//If we can't see any enemies then return to patrolling
	if (enemy->GetIsEnemyInSight() == false)
	{
		enemy->GetFSM()->ChangeState(PatrolBetweenTwoPoints::Instance());
	}
	else if (enemy->GetPosition() == enemy->GetTargetDestination()) //No need to check for los as we already have in previous statement
	{
		//Continue fleeing
		enemy->SetPathToFollow(PathPlanner::SearchForFleePath(enemy->GetLevelGraph(), enemy->GetPosition().x,
			enemy->GetPosition().z, enemy->GetEnemyInSight()->GetPosition().x,
			enemy->GetEnemyInSight()->GetPosition().z, false, 20));
	}

	if (enemy->GetMovementTimer() > enemy->GetMovementDelay())
	{
		enemy->MoveOnPath();
		enemy->ResetMovementTimer();
	}
	
}

void FleeFromPlayer::Exit(Enemy* enemy)
{
	//Debug text
}

//////CHASE ENEMY//////
ChasePlayer* ChasePlayer::Instance()
{
	static ChasePlayer instance;

	return &instance;
}
void ChasePlayer::Enter(Enemy* enemy)
{
	//plot a path to the enemy
	enemy->SetPathToFollow(PathPlanner::Search(enemy->GetLevelGraph(), enemy->GetPosition().x,
		enemy->GetPosition().z, enemy->GetEnemyInSight()->GetPosition().x,
		enemy->GetEnemyInSight()->GetPosition().z, false));

}

void ChasePlayer::Update(Enemy* enemy)
{
	enemy->CheckForEnemyInSight();

	//first we check to see if the enemy is still in sight, if not we continue to their last known position
	//if we can still see the enemy we check to see if they have moved, if so we update our path to go to their new position.
	if (enemy->GetIsEnemyInSight() == true && enemy->GetEnemyInSight()->GetPosition() != enemy->GetTargetDestination())
	{
		//Update path and continue chasing
		enemy->SetPathToFollow(PathPlanner::Search(enemy->GetLevelGraph(), enemy->GetPosition().x,
			enemy->GetPosition().z, enemy->GetEnemyInSight()->GetPosition().x,
			enemy->GetEnemyInSight()->GetPosition().z, false));

	}
	else if (enemy->GetIsEnemyInSight() == true && enemy->GetEnemyInSight()->GetHitpoints() <= 0)
	{
		std::cout << "Got 'em!" << std::endl;
		enemy->ClearTargetEnemy();
		enemy->GetFSM()->ChangeState(PatrolBetweenTwoPoints::Instance());
	}
	//if enemy is in line of sight && range is < enemy range run away
	else if (enemy->GetIsEnemyInSight() == true && enemy->GetWeaponRange() < enemy->GetEnemyInSight()->GetWeaponRange())
	{
		//Run away from enemy
		enemy->GetFSM()->ChangeState(FleeFromPlayer::Instance());
	}
	if (enemy->GetMovementTimer() > enemy->GetMovementDelay())
	{
		enemy->MoveOnPath();
		enemy->ResetMovementTimer();
	}
}

void ChasePlayer::Exit(Enemy* enemy)
{
	//Debug text
}

//////GLOBAL FIRE STATE//////
EnemyFire* EnemyFire::Instance()
{
	static EnemyFire instance;

	return &instance;
}
void EnemyFire::Enter(Enemy* enemy)
{
}

void EnemyFire::Update(Enemy* enemy)
{
	if (enemy->CanPlayerAttackAnEnemy() && enemy->GetWeaponCooldown() > enemy->GetWeaponRechargeTime())
		enemy->FireShotAtEnemy();
}

void EnemyFire::Exit(Enemy* enemy)
{
	//Debug text
}