#include "pch.h"
#include "EnemyStatePool.h"

#include "Actors/Enemies/EnemyStates/EnemyStateHeaders.h"


EnemyState* EnemyStatePool::createNewObjects(EnemyState::Type type, int count, int& outSize) const
{
	EnemyState* state = nullptr;

	switch (type)
	{
	case EnemyState::Wait:
		state = new EnemyWait[count];
		outSize = sizeof(EnemyWait);
		break;
	case EnemyState::Idle:
		state = new EnemyIdle[count];
		outSize = sizeof(EnemyIdle);
		break;
	case EnemyState::Run:
		state = new EnemyRun[count];
		outSize = sizeof(EnemyRun);
		break;
	case EnemyState::Patrol:
		state = new EnemyPatrol[count];
		outSize = sizeof(EnemyPatrol);
		break;
	case EnemyState::Alert:
		state = new EnemyAlert[count];
		outSize = sizeof(EnemyAlert);
		break;
	case EnemyState::Hit:
		state = new EnemyHit[count];
		outSize = sizeof(EnemyHit);
		break;
	case EnemyState::Stun:
		state = new EnemyStun[count];
		outSize = sizeof(EnemyStun);
		break;
	case EnemyState::PreAttack:
		state = new EnemyPreAttack[count];
		outSize = sizeof(EnemyPreAttack);
		break;
	case EnemyState::Attack:
		state = new EnemyAttack[count];
		outSize = sizeof(EnemyAttack);
		break;
	case EnemyState::Charge:
		state = new EnemyCharge[count];
		outSize = sizeof(EnemyCharge);
		break;
	case EnemyState::Dead:
		state = new EnemyDead[count];
		outSize = sizeof(EnemyDead);
		break;
	case EnemyState::Exit:
		state = new EnemyExit[count];
		outSize = sizeof(EnemyExit);
	break;	
	case EnemyState::None:
	default:
		DebugPrint(Warning, "Invalid enemy state %d, could not create %d objects\n", type, count);
		break;
	}


	return state;
}