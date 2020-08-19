#include "pch.h"
#include "EnemyDebugger.h"

#include "Debug/DebugDraw.h"

#include "Game/Environment.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/PlayerManager.h"

#include "Enemy.h"



void EnemyDebugger::draw()
{
#if LABEL_ENEMY_STATES
	labelState();
#endif
#if LABEL_ENEMY_HEALTH
	labelHP();
#endif
#if DRAW_ENEMY_RECTS
	drawRects();
#endif
#if DRAW_PLAYER_ENEMY_DISTANCE
	mDebugger.drawPlayerDistance(mEnvironment, this);
#endif
#if DRAW_AI_PATH
	drawPath();
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


void EnemyDebugger::drawPlayerDistance(Environment* environment, const Enemy* enemy)
{
	VectorF playerPosition = environment->actors()->player()->get()->position();
	VectorF enemyPosition = mEnemy->position();
	debugDrawLine(playerPosition, enemyPosition, RenderColour::Red);

	int ptSize = 20;
	VectorF position = (playerPosition + enemyPosition) / 2.0f;
	RenderColour colour = RenderColour::Green;
	float playerDistance = distance(playerPosition, enemyPosition);
	const BasicString playerDistanceString(playerDistance, 2);
	debugRenderText(playerDistanceString, ptSize, position, colour);
}


void EnemyDebugger::drawPath()
{
	//if (mEnemy->state() == EnemyState::Run)
	{
		//EnemyRun* enemyRun = static_cast<EnemyRun*>(&(mEnemy->getStateMachine()->getActiveState()));
		Path path = mPath;

		std::vector<PathTile> enemyPath;
		const AIPathMap* map = mEnemy->getPathMap().pathMap();

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
}

