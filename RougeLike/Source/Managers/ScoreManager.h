#pragma once

#include "Events/Observer.h"

class ScoreManager : public Observer
{
public:
	ScoreManager() : mScore(0) { }

	void handleEvent(Event event, EventData& data);

	int score() { return mScore; }
private:
	int mScore;
};