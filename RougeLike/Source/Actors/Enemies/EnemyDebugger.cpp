#include "pch.h"
#include "EnemyDebugger.h"

#include "Debug/DebugDraw.h"

#include "Game/Environment.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/PlayerManager.h"

#include "Enemy.h"


void EnemyDebugger::labelState(EnemyState::Type state, RectF enemyRect) const
{
	const BasicString& enemyState = enemyStateLookUpTable.at(state);
	int ptSize = 16;
	VectorF position = enemyRect.TopCenter();
	RenderColour colour = RenderColour::Red;
	debugRenderText(enemyState, ptSize, position, colour);
}


void EnemyDebugger::drawPlayerDistance(Environment* environment, const Enemy* enemy) const
{
	VectorF playerPosition = environment->actors()->player()->get()->position();
	VectorF enemyPosition = enemy->position();
	debugDrawLine(playerPosition, enemyPosition, RenderColour::Red);

	int ptSize = 20;
	VectorF position = (playerPosition + enemyPosition) / 2.0f;
	RenderColour colour = RenderColour::Green;
	float playerDistance = distance(playerPosition, enemyPosition);
	const BasicString playerDistanceString(playerDistance, 2);
	debugRenderText(playerDistanceString, ptSize, position, colour);
}