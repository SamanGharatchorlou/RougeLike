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


void ScoreManager::handleEvent(const EventPacket eventPacket)
{
	hasChanged = false;
	int currentScore = mScore;

	switch (eventPacket.event)
	{
	case Event::EnemyDead:
	{
		const EnemyDeadEvent* deathData = static_cast<const EnemyDeadEvent*>(eventPacket.data);
		mScore += deathData->mScore;
		break;
	}
	default:
		break;
	}


	hasChanged = (currentScore != mScore);
}