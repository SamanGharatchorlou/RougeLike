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
		notify(Event::UpdateTextBox, scoreEvent);

		UpdateTextBoxEvent mapLevelEvent("MapLevel val", mMapLevel);
		notify(Event::UpdateTextBox, mapLevelEvent);
	}

	updateUI = false;
}


void ScoreManager::handleEvent(const Event event, EventData& data)
{
	switch (event)
	{
	case Event::EnemyDead:
	{
		EnemyDeadEvent deathData = static_cast<EnemyDeadEvent&>(data);
		mScore += deathData.mScore;
		break;
	}
	case Event::IncrementMapLevel:
	{
		mMapLevel++;
	}
	default:
		break;
	}

	updateUI = true;
}