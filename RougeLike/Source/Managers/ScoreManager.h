#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

class ScoreManager : public Observer, public Dispatcher
{
public:
	ScoreManager();
	
	void reset();

	void slowUpdate();

	void handleEvent(const Event event, EventData& data) override;

	int score() { return mScore; }

	bool UIRequiresUpdate() { return updateUI; }

private:
	int mScore;
	int mMapLevel;

	bool updateUI;
};