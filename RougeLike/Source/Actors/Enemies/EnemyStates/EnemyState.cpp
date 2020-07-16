#include "pch.h"
#include "EnemyState.h"

#include "EnemyStateHeaders.h"


EnemyState* newEnemyState(EnemyState::Type type, Enemy* enemy)
{
	EnemyState* state = nullptr;

	switch (type)
	{
	case EnemyState::Wait:
		state = new EnemyWait(enemy);
		break;

	case EnemyState::Idle:
		state = new EnemyIdle(enemy);
		break;

	case EnemyState::Run:
		state = new EnemyRun(enemy);
		break;

	case EnemyState::Patrol:
		state = new EnemyPatrol(enemy);
		break;

	case EnemyState::Alert:
		state = new EnemyAlert(enemy);
		break;

	case EnemyState::Hit:
		state = new EnemyHit(enemy);
		break;

	case EnemyState::PreAttack:
		state = new EnemyPreAttack(enemy);
		break;

	case EnemyState::Attack:
		state = new EnemyAttack(enemy);
		break;

	case EnemyState::Dead:
		state = new EnemyDead(enemy);
		break;

	case EnemyState::Exit:
		state = new EnemyExit;
		break;

	case EnemyState::None:
	default:
		DebugPrint(Warning, "No enemy state set, no state was replaced\n");
		break;
	}

	return state;
}