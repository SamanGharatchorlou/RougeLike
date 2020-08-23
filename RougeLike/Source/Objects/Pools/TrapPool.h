#pragma once

#include "Utilities/ObjectPool.h"
#include "Map/TileInteractables/TileInteractable.h"

class Trap;
class Trigger;
enum class DecorType : Uint32;


class TrapPool : public ObjectPool<TileInteractable, DecorType>
{
public:
	Trap* getTrap(DecorType trapType);
	Trigger* getTrigger(DecorType triggerType);

private:
	TileInteractable* createNewObject(DecorType type) const override;
};