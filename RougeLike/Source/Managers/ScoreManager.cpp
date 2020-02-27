#include "pch.h"
#include "ScoreManager.h"

void ScoreManager::update()
{
	if (hasChanged)
	{
		UpdateScoreEvent event(mScore);
		notify(Event::UpdateScore, event);
	}

	hasChanged = false;
}


void ScoreManager::handleEvent(const Event event, EventData& data)
{
	hasChanged = false;
	int currentScore = mScore;

	switch (event)
	{
	case Event::EnemyDead:
	{
		EnemyDeadEvent deathData = static_cast<EnemyDeadEvent&>(data);
		mScore += deathData.mScore;
		break;
	}
	default:
		break;
	}


	hasChanged = (currentScore != mScore);
}