#pragma once

#include "Utilities/ObjectPool.h"

class TileInteractable;
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