#include "pch.h"
#include "ScoreManager.h"


ScoreManager::ScoreManager() : mScore(0), mMapLevel(0), updateUI(true)
{

}


void ScoreManager::reset()
{
	mScore = 0;
	mMapLevel = 0;
	updateUI = true;
}


void ScoreManager::slowUpdate()
{
	if (updateUI)
	{
		UpdateTextBoxEvent scoreEvent("Score val", mScore);
		notify(scoreEvent);

		UpdateTextBoxEvent mapLevelEvent("MapLevel val", mMapLevel);
		notify(mapLevelEvent);
	}

	updateUI = false;
}


void ScoreManager::handleEvent(EventData& data)
{
	switch (data.eventType)
	{
	case Event::EnemyDead:
	{
		EnemyDeadEvent deathData = static_cast<EnemyDeadEvent&>(data);
		mScore += (int)deathData.mScore;

		updateUI = true;
		break;
	}
	case Event::IncrementMapLevel:
	{
		mMapLevel++;

		updateUI = true;
	}
	default:
		break;
	}

}