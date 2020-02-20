#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

class ScoreManager : public Observer, public Dispatcher
{
public:
	ScoreManager() : mScore(0), hasChanged(false) { }

	void update();
	void handleEvent(Event event, EventData& data);

	int score() { return mScore; }

	bool hasScoreChanged() { return hasChanged; }

private:
	int mScore;

	bool hasChanged;
};