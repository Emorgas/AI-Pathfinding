#include "PlayerStates.h"
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"
//////COLLECT CHARGE PACK//////
CollectChargePack* CollectChargePack::Instance()
{
	static CollectChargePack instance;

	return &instance;
}
void CollectChargePack::Enter(Player* player)
{
	std::cout << "Searching for a Charge Pack!" << std::endl;
	//Find the nearest objective
	position pos = player->GetNearestObjective();
	if (pos.x != -1 && pos.z != -1)
	{
		//Plot a path to it
		player->SetPathToFollow(PathPlanner::Search(player->GetLevelGraph(), player->GetPosition().x, player->GetPosition().z,
			pos.x, pos.z, true));
	}
	else
	{
		std::cout << "Oops! Forgot I already had them all..." << std::endl;
		player->GetFSM()->SetCurrentState(ExitLevel::Instance());
	}
}

void CollectChargePack::Update(Player* player)
{
	player->CheckForEnemyInSight();

	position pos = player->GetNearestObjective();
	if (pos.x != -1 && pos.z != -1)
	{
		player->SetPathToFollow(PathPlanner::Search(player->GetLevelGraph(), player->GetPosition().x, player->GetPosition().z,
			pos.x, pos.z, true));	//Plot a path to it
	}

	//if enemy is in line of sight && range is < enemy range run away
	if (player->GetIsEnemyInSight() == true && player->GetWeaponRange() < player->GetEnemyInSight()->GetWeaponRange())
	{
		//Run away from enemy
		player->GetFSM()->ChangeState(FleeFromEnemy::Instance());
	}
	//if enemy is in line of sight && range is > enemy range engage
	else if (player->GetIsEnemyInSight() == true && player->GetWeaponRange() > player->GetEnemyInSight()->GetWeaponRange())
	{
		//Chase the enemy down
		player->GetFSM()->ChangeState(ChaseEnemy::Instance());
	}
	//if the player has reached their target position
	else if (player->GetPosition() == player->GetTargetDestination())
	{
		player->CollectChargePack();
		std::cout << "I have collected a charge pack!" << std::endl;
		//Find the next nearest objective
		position pos = player->GetNearestObjective();
		if (pos.x != -1 && pos.z != -1)
		{
			std::cout << "Searching for another..." << std::endl;
			player->SetPathToFollow(PathPlanner::Search(player->GetLevelGraph(), player->GetPosition().x, player->GetPosition().z,
				pos.x, pos.z, true));	//Plot a path to it
		}
		else
			player->GetFSM()->ChangeState(ExitLevel::Instance());
	}

	//Move towards pack
	if (player->GetMovementTimer() > player->GetMovementDelay())
	{
		player->MoveOnPath();
		player->ResetMovementTimer();
	}
}

void CollectChargePack::Exit(Player* player)
{
	//Debug text
	std::cout << "All Charge Packs Collected! Weapon at Maximum Power!" << std::endl;
}

//////FLEE FROM ENEMY//////
FleeFromEnemy* FleeFromEnemy::Instance()
{
	static FleeFromEnemy instance;

	return &instance;
}
void FleeFromEnemy::Enter(Player* player)
{
	std::cout << "Stronger Enemy Encountered! Running Away!" << std::endl;
	player->SetPathToFollow(PathPlanner::SearchForFleePath(player->GetLevelGraph(), player->GetPosition().x,
		player->GetPosition().z, player->GetEnemyInSight()->GetPosition().x,
		player->GetEnemyInSight()->GetPosition().z, true, 20));
}

void FleeFromEnemy::Update(Player* player)
{
	player->CheckForEnemyInSight();

	//If we can't see any enemies then return to collecting charge packs
/*	if (player->GetIsEnemyInSight() == false)
	{
		std::cout << "I think I lost them! Returning to my search for charge packs!" << std::endl;
		player->GetFSM()->ChangeState(CollectChargePack::Instance());
	}*/
	if (player->GetPosition() == player->GetTargetDestination() && player->GetIsEnemyInSight() == true) //No need to check for los as we already have in previous statement
	{
		//Continue fleeing
		player->SetPathToFollow(PathPlanner::SearchForFleePath(player->GetLevelGraph(), player->GetPosition().x,
			player->GetPosition().z, player->GetEnemyInSight()->GetPosition().x,
			player->GetEnemyInSight()->GetPosition().z, true, 20));
	}
	else if (player->GetPosition() == player->GetTargetDestination())
	{
		player->GetFSM()->ChangeState(CollectChargePack::Instance());
	}

	if (player->GetMovementTimer() > player->GetMovementDelay())
	{
		player->MoveOnPath();
		player->ResetMovementTimer();
	}
	
}

void FleeFromEnemy::Exit(Player* player)
{
	//Debug text
}

//////CHASE ENEMY//////
ChaseEnemy* ChaseEnemy::Instance()
{
	static ChaseEnemy instance;

	return &instance;
}
void ChaseEnemy::Enter(Player* player)
{
	//plot a path to the enemy
	std::cout << "Target Sighted. Engaging!" << std::endl;
	Path* tempPath = PathPlanner::Search(player->GetLevelGraph(), player->GetPosition().x,
		player->GetPosition().z, player->GetEnemyInSight()->GetPosition().x,
		player->GetEnemyInSight()->GetPosition().z, true);
	tempPath->path.erase(tempPath->path.end() - 1); //Erase the last position in the path to avoid standing on the player
	player->SetPathToFollow(tempPath);

}

void ChaseEnemy::Update(Player* player)
{
	player->CheckForEnemyInSight();

	//first we check to see if the enemy is still in sight, if not we continue to their last known position
	//if we can still see the enemy we check to see if they have moved, if so we update our path to go to their new position.
	if (player->GetIsEnemyInSight() == true && player->GetEnemyInSight()->GetPosition() != player->GetTargetDestination())
	{
		//Update path and continue chasing
		Path* tempPath = PathPlanner::Search(player->GetLevelGraph(), player->GetPosition().x,
			player->GetPosition().z, player->GetEnemyInSight()->GetPosition().x,
			player->GetEnemyInSight()->GetPosition().z, true);
		tempPath->path.erase(tempPath->path.end() - 1); //Erase the last position in the path to avoid standing on the player
		player->SetPathToFollow(tempPath);
	}
	else if (player->GetIsEnemyInSight() == true && player->GetEnemyInSight()->GetHitpoints() <= 0)
	{
		std::cout << "Got 'em!" << std::endl;
		player->ClearTargetEnemy();
		player->GetFSM()->ChangeState(CollectChargePack::Instance());
	}
	//if enemy is in line of sight && range is < enemy range run away
	else if (player->GetIsEnemyInSight() == true && player->GetWeaponRange() < player->GetEnemyInSight()->GetWeaponRange())
	{
		//Run away from enemy
		player->GetFSM()->ChangeState(FleeFromEnemy::Instance());
	}
	else if (player->GetPathToFollow()->path.size() <= 0)
	{
		player->GetFSM()->ChangeState(CollectChargePack::Instance());
	}

	if (player->GetMovementTimer() > player->GetMovementDelay())
	{
		player->MoveOnPath();
		player->ResetMovementTimer();
	}
}

void ChaseEnemy::Exit(Player* player)
{
	//Debug text
}

//////EXIT LEVEL//////
ExitLevel* ExitLevel::Instance()
{
	static ExitLevel instance;

	return &instance;
}
void ExitLevel::Enter(Player* player)
{
	std::cout << "I'm outta here!" << std::endl;
	player->SetPathToFollow(PathPlanner::Search(player->GetLevelGraph(), player->GetPosition().x,
		player->GetPosition().z, player->GetLevelExitPosition().x,
		player->GetLevelExitPosition().z, true));
}

void ExitLevel::Update(Player* player)
{
	player->CheckForEnemyInSight();

	//if enemy is in line of sight && range is < enemy range run away
	if (player->GetIsEnemyInSight() == true && player->GetWeaponRange() < player->GetEnemyInSight()->GetWeaponRange())
	{
		//Run away from enemy
		player->GetFSM()->ChangeState(FleeFromEnemy::Instance());
	}
	//if enemy is in line of sight && range is > enemy range engage
	else if (player->GetIsEnemyInSight() == true && player->GetWeaponRange() > player->GetEnemyInSight()->GetWeaponRange())
	{
		//Chase the enemy down
		player->GetFSM()->ChangeState(ChaseEnemy::Instance());
	}
	else if (player->GetPosition() == player->GetLevelExitPosition())
	{
		std::cout << "Level Complete!" << std::endl;
	}

	player->SetPathToFollow(PathPlanner::Search(player->GetLevelGraph(), player->GetPosition().x,
		player->GetPosition().z, player->GetLevelExitPosition().x,
		player->GetLevelExitPosition().z, true));

	if (player->GetMovementTimer() > player->GetMovementDelay())
	{
		player->MoveOnPath();
		player->ResetMovementTimer();
	}
}

void ExitLevel::Exit(Player* player)
{
	//Debug text
	std::cout << "Gotta go chase someone!" << std::endl;
}

//////GLOBAL FIRE STATE//////
Fire* Fire::Instance()
{
	static Fire instance;

	return &instance;
}
void Fire::Enter(Player* player)
{
}

void Fire::Update(Player* player)
{
	if (player->CanPlayerAttackAnEnemy() && player->GetWeaponCooldown() > player->GetWeaponRechargeTime())
		player->FireShotAtEnemy();
}

void Fire::Exit(Player* player)
{
	//Debug text
}