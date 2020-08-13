#pragma once

#include "TileInteractable.h"

class Trap;

class Trigger : public TileInteractable
{
public:
	virtual ~Trigger() { }

	void clear() override
	{
		mTraps = nullptr;
		TileInteractable::clear();
	}

	void linkTraps(std::vector<Trap*>* traps) { mTraps = traps; }

	bool canTrigger() const { return !exhausted && mTraps && gotHit(); }


protected:
	std::vector<Trap*>* mTraps;
};

