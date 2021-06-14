#include "pch.h"
#include "EnemyDebugger.h"

#if DEUBG_CHECK
#include "Game/Environment.h"
#include "AI/Pathing/AIPathMap.h"

#include "Actors/ActorManager.h"
#include "Actors/Player/PlayerManager.h"

#include "Enemy.h"
#include "Collisions/Colliders/Collider.h"


void EnemyDebugger::draw()
{
#if TRACK_COLLISIONS
	mEnemy->collider()->renderCollider();
#endif
#if LABEL_ENEMY_HEALTH
	labelHP();
#endif
#if DRAW_ENEMY_RECTS
	drawRects();
#endif
#if DRAW_PLAYER_ENEMY_DISTANCE
	drawTargetDistance();
#endif
#if DRAW_AI_PATH
	drawPath();
#endif
#if LABEL_ENEMY_STATES
	labelState();
#endif
}


void EnemyDebugger::labelState()
{
	EnemyState::Type type = mEnemy->getStateMachine()->getActiveState().type();
	const BasicString& enemyState = enemyStateLookUpTable.at(type);
	int ptSize = 16;
	VectorF position = mEnemy->rect().TopCenter();
	RenderColour colour = RenderColour::Red;
	debugRenderText(enemyState, ptSize, position, colour);
}


void EnemyDebugger::labelHP()
{
	float hp = mEnemy->getAttributeValue(PropertyType::Health);
	BasicString health = "HP: " + BasicString(hp);

	int ptSize = 14;
	VectorF position = mEnemy->rect().TopCenter();
	RenderColour colour = RenderColour::Red;
	debugRenderText(health, ptSize, position, colour);
}


void EnemyDebugger::drawRects()
{
	debugDrawRect(mEnemy->rect(), RenderColour(RenderColour::Red));
	debugDrawRect(mEnemy->scaledRect(), RenderColour(RenderColour::Blue));
}


void EnemyDebugger::drawTargetDistance()
{
	if (mEnemy->hasTarget())
	{
		VectorF targetPosition = mEnemy->target()->position();
		VectorF enemyPosition = mEnemy->position();
		debugDrawLine(targetPosition, enemyPosition, RenderColour::Red);

		int ptSize = 20;
		VectorF position = (targetPosition + enemyPosition) / 2.0f;
		RenderColour colour = RenderColour::Green;
		float playerDistance = distance(targetPosition, enemyPosition);
		const BasicString playerDistanceString(playerDistance, 2);
		debugRenderText(playerDistanceString, ptSize, position, colour);
	}
}


void EnemyDebugger::drawPath()
{
	// if not in run state path should be size 0
	Path path = mPath;

	std::vector<PathTile> enemyPath;
	const AIPathMap* map = mEnemy->pathing()->map();

	while (path.size() > 0)
	{
		Index index = path.top();
		path.pop();

		PathTile tile = *map->tile(index);
		enemyPath.push_back(tile);
	}

	if (enemyPath.size() > 1)
	{
		for (int i = 0; i < enemyPath.size() - 1; i++)
		{
			RectF rect = enemyPath[i].rect();

			VectorF pointA = enemyPath[i].rect().Center();
			VectorF pointB = enemyPath[i + 1].rect().Center();

			debugDrawLine(pointA, pointB, RenderColour::Green);
		}
	}
}
#endif
