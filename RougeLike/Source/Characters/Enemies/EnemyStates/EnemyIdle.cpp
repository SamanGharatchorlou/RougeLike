#include "pch.h"
#include "EnemyIdle.h"

#include "Characters/Enemies/Enemy.h"
#include "Map/MapLevel.h"
#include "Map/Map.h"


void EnemyIdle::init()
{
	timer.restart();
	mEnemy->getAnimator()->selectAnimation("Idle");

	mEnemy->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}

void EnemyIdle::fastUpdate(float dt)
{
	mEnemy->resolvePlayerWeaponCollisions();
}


void EnemyIdle::slowUpdate(float dt)
{
	float idleTime = mEnemy->propertyBag().pIdleTime.get();;

	if (idleTime > 0 && timer.getSeconds() > idleTime)
		mEnemy->replaceState(EnemyState::Patrol);

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


bool EnemyIdle::canSeeAttackTarget() const
{
	Map* map = mEnemy->getData()->level->primaryMap();

	VectorF position = mEnemy->position();
	VectorF attackTargetPosition = mEnemy->attackTargetRect()->Center();

	bool isNearby = distanceSquared(attackTargetPosition, position) < mEnemy->propertyBag().pSightRange.get();
	bool hasLineOfSight = false;

	if (!isNearby)
	{
		// Is there a line of sight?
		float yPatrolDirection = (mEnemy->positionTargetRect()->Center() - position).y;
		float yTargetDirection = (attackTargetPosition - position).y;

		bool isFacingTarget =
			(yPatrolDirection > 0.0f && yTargetDirection > 0.0f) ||
			(yPatrolDirection < 0.0f && yTargetDirection < 0.0f);

		if (isFacingTarget)
		{
			int enemyXTile = map->index(position).x;
			int playerXTile = map->index(attackTargetPosition).x;
			hasLineOfSight = (enemyXTile == playerXTile) || (enemyXTile == playerXTile + 1) || (enemyXTile == playerXTile - 1);
		}

	}

	return isNearby || hasLineOfSight;
}


void EnemyIdle::render()
{
	mEnemy->renderCharacter();
}