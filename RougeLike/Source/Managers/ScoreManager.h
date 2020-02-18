#pragma once

#include "Events/Observer.h"

class ScoreManager : public Observer
{
public:
	ScoreManager() : mScore(0), hasChanged(false) { }

	void handleEvent(Event event, EventData& data);

	int score() { return mScore; }

	bool hasScoreChanged() { return hasChanged; }

private:
	int mScore;

	bool hasChanged;
};