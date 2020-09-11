#include "pch.h"
#include "ScoreManager.h"


ScoreManager::ScoreManager() : mScore(0), mMapLevel(0), updateUI(false), mNewLevelPopuped(false)
{

}


void ScoreManager::reset()
{
	mScore = 0;
	mMapLevel = 0;
	updateUI = false;
}


void ScoreManager::slowUpdate()
{
	if (updateUI)
	{
		UpdateTextBoxEvent scoreEvent("Score val", BasicString((float)mScore));
		notify(scoreEvent);

		UpdateTextBoxEvent mapLevelEvent("MapLevel val", BasicString((float)mMapLevel));
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

		UpdateTextBoxEvent event = UpdateTextBoxEvent("MapLevel val", BasicString((float)mMapLevel));
		notify(event);

		// Open level 1 popup
		if (!mNewLevelPopuped)
		{
			if (mMapLevel == 1)
			{
				OpenPopupEvent newLevel("NewLevel");
				notify(newLevel);
				mNewLevelPopuped = true;
			}
		}

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