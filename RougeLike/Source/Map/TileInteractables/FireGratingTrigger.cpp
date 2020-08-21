#include "pch.h"
#include "FireGratingTrigger.h"

#include "Map/Tiles/MapTile.h"
#include "Map/Tiles/TileTypes.h"
#include "FireGratingTrap.h"



void FireGratingTrigger::trigger()
{
	Animator& animator = mTile->animation(0);
	animator.getAnimation(Action::Active).nextFrame();

	exhausted = true;

	for (int i = 0; i < mTraps->size(); i++)
	{
		Trap* trap = mTraps->at(i);
		if (trap->tile()->has(DecorType::Grating))
		{
			FireGratingTrap* fireTrap = static_cast<FireGratingTrap*>(trap);
			fireTrap->setActive(true);
			fireTrap->set();
		}
	}
}
