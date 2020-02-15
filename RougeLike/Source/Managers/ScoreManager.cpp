#include "pch.h"
#include "ScoreManager.h"

// testing - dont need
#include "Events/Observer.h"

void ScoreManager::handleEvent(Event event, EventData& data)
{
	switch (event)
	{
	case Event::None:
		break;
	case Event::EnemyDead:
	{

		//EventData* dataPtr = &data;

		//EnemyDeadData& deathData = dynamic_cast<EnemyDeadData&>(data);
		//int enemyScore = deathData.score;

		//mScore += enemyScore;

		// blah blah

		int a = 4;

		printf("score is now %d\n", mScore);
		break;
	}
	default:
		break;
	}
}