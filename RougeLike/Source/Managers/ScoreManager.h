#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

// TODO: fix this
class ScoreManager : public Observer, public Dispatcher
{
public:
	ScoreManager();
	
	void reset();

	void slowUpdate();

	void handleEvent(EventData& data) override;

	int score() { return mScore; }

	bool UIRequiresUpdate() { return updateUI; }

private:
	int mScore;
	int mMapLevel;
	int mKills;

	bool updateUI;
};