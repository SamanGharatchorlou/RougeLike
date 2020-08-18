#include "pch.h"
#include "EnemyStatePool.h"

#include "Actors/Enemies/EnemyStates/EnemyStateHeaders.h"


EnemyState* EnemyStatePool::createNewObject(EnemyState::Type type) const
{
	EnemyState* state = nullptr;

	switch (type)
	{
	case EnemyState::Wait:
		state = new EnemyWait;
		break;
	case EnemyState::Idle:
		state = new EnemyIdle;
		break;
	case EnemyState::Run:
		state = new EnemyRun;
		break;
	case EnemyState::Patrol:
		state = new EnemyPatrol;
		break;
	case EnemyState::Alert:
		state = new EnemyAlert;
		break;
	case EnemyState::Hit:
		state = new EnemyHit;
		break;
	case EnemyState::Stun:
		state = new EnemyStun;
		break;
	case EnemyState::PreAttack:
		state = new EnemyPreAttack;
		break;
	case EnemyState::Attack:
		state = new EnemyAttack;
		break;
	case EnemyState::Dead:
		state = new EnemyDead;
		break;
	case EnemyState::Exit:
		state = new EnemyExit;
	break;	
	case EnemyState::None:
	default:
		DebugPrint(Warning, "No enemy state with type %d\n", type);
		break;
	}

	return state;
}