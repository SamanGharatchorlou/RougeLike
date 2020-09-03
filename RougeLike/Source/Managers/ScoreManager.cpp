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
		mKills++;

		updateUI = true;
		break;
	}
	case Event::MapLevel:
	{
		MapLevelEvent mapLevelEvent = static_cast<MapLevelEvent&>(data);
		mMapLevel = mapLevelEvent.mMapLevel;

		UpdateTextBoxEvent event = UpdateTextBoxEvent("MapLevel val", BasicString(mMapLevel));
		notify(event);
		break;
	}
	case Event::PlayerDead:
	{
		PlayerDeadEvent event(mKills, mScore, mMapLevel);
		notify(event);
		break;
	}
	default:
		break;
	}

}