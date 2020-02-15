#include "pch.h"
#include "ScoreManager.h"

void ScoreManager::handleEvent(Event event, EventData& data)
{
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
}