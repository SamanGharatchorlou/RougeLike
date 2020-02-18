#include "pch.h"
#include "ScoreManager.h"

void ScoreManager::handleEvent(Event event, EventData& data)
{
	hasChanged = false;
	int currentScore = mScore;

	switch (event)
	{
	case Event::None:
		break;
	case Event::EnemyDead:
	{
		EnemyDeadData& deathData = static_cast<EnemyDeadData&>(data);
		mScore += deathData.score;
		break;
	}
	default:
		break;
	}

	hasChanged = (currentScore != mScore);
}