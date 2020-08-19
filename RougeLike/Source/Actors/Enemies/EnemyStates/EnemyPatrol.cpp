#include "pch.h"
#include "EnemyPatrol.h"

#include "AI/AIPathMap.h"
#include "Actors/Enemies/Enemy.h"



void EnemyPatrol::init()
{
	mEnemy->animator().selectAnimation(Action::Walk);
	setPatrolPoint();


	//const AIPathMap* map = mEnemy->getPathMap().pathMap();
	//mIndexTarget = map->index(mEnemy->position());
	//mIndexTarget.y = 0;

	//mPositionTarget = mEnemy->position();
	//mPositionTarget.y = 0.0f;
}


void EnemyPatrol::fastUpdate(float dt)
{
	//if()

	mEnemy->accellerateTowards(mPositionTarget);
}


void EnemyPatrol::slowUpdate(float dt)
{
	mEnemy->resolveCollisions();

	if (hasReachedPositionTarget())
		mEnemy->idle(1.0f);

	if (canSeeAttackTarget())
		mEnemy->replaceState(EnemyState::Alert);
}


void EnemyPatrol::render()
{
	mEnemy->renderCharacter();
}


void EnemyPatrol::resume()
{
	mEnemy->animator().selectAnimation(Action::Walk);
	setPatrolPoint();
}


// --- Private Functions -- //

void EnemyPatrol::setPatrolPoint()
{
	const AIPathMap* map = mEnemy->getPathMap().pathMap();

	VectorF position = mEnemy->position();
	Index index = map->index(position);

	Vector2D<int> yTileRange = map->yTileFloorRange(index);

	VectorF highestPoint = map->tile(Index(index.x, yTileRange.x))->rect().BotCenter();
	VectorF lowestPoint = map->tile(Index(index.x, yTileRange.y))->rect().TopCenter();

	// set the furthest point
	VectorF patrolTarget = (position.y - highestPoint.y < lowestPoint.y - position.y) ? lowestPoint : highestPoint;
	ASSERT(Warning, map->isValidPosition(patrolTarget), "Invalid enemy patrol target: %f, %f", patrolTarget.x, patrolTarget.y);

	const PathTile* tile = map->tile(patrolTarget);
	mPositionTarget = tile->rect().Center();
}



bool EnemyPatrol::canSeeAttackTarget() const
{
	if (mEnemy->hasTarget())
	{
		VectorF position = mEnemy->position();
		VectorF target = mEnemy->target()->position();

		return distance(target, position) < mEnemy->getAttributeValue(AttributeType::SightRange);
	}
	else
		return false;
}


bool EnemyPatrol::hasReachedPositionTarget() const
{
	VectorF position = mEnemy->position();
	return distanceSquared(position, mPositionTarget) < 10.0f;
}


//bool EnemyPatrol::canMove(VectorF velocity, float dt)
//{
//	const AIPathMap* map = mEnemy->getPathMap().pathMap();
//	mIndexTarget = map->index(mEnemy->position());
//
//
//	bool validMovement = true;
//	RectF rect = mEnemy->scaledRect().Translate(velocity * dt);
//	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };
//
//	for (int i = 0; i < 4; i++)
//	{
//		if (!mEnemy->getPathMap().pathMap()->isValidPosition(points[i]))
//			validMovement = false;
//	}
//
//	return validMovement;
//}