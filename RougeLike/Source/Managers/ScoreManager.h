#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

class ScoreManager : public Observer, public Dispatcher
{
public:
	ScoreManager() : mScore(0), hasChanged(false) { }

	void slowUpdate();

	void handleEvent(const Event event, EventData& data) override;

	int score() { return mScore; }

	bool hasScoreChanged() { return hasChanged; }

private:
	int mScore;

	bool hasChanged;
};