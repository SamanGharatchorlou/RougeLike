#include "pch.h"
#include "TrapPool.h"

#include "Map/Tiles/TileTypes.h"

#include "Map/TileInteractables/SpikeTrap.h"
#include "Map/TileInteractables/FireGratingTrap.h"
#include "Map/TileInteractables/FireGratingTrigger.h"


Trap* TrapPool::getTrap(DecorType trapType)
{
#if _DEBUG
	if (!(trapType > DecorType::TRAP && trapType < DecorType::TRIGGER))
	{
		DebugPrint(Warning, "Trap type %d is not a trap\n", trapType);
		return nullptr;
	}
#endif

	TileInteractable* trap = getObject(trapType);
	return static_cast<Trap*>(trap);
}

Trigger* TrapPool::getTrigger(DecorType triggerType)
{
#if _DEBUG
	if (!(triggerType > DecorType::TRIGGER && triggerType < DecorType::END))
	{
		DebugPrint(Warning, "Trigger type %d is not a trigger\n", triggerType);
		return nullptr;
	}
#endif

	TileInteractable* trap = getObject(triggerType);
	return static_cast<Trigger*>(trap);
}


TileInteractable* TrapPool::createNewObject(DecorType type) const
{
	TileInteractable* interactable = nullptr;

	switch (type)
	{
	case DecorType::Spikes:
		interactable = new SpikeTrap;
		break;	
	case DecorType::Grating:
		interactable = new FireGratingTrap;
		break;
	case DecorType::GratingTrigger:
		interactable = new FireGratingTrigger;
		break;
	case DecorType::None:
	case DecorType::Column:
	case DecorType::Water:
	case DecorType::Torch:
	case DecorType::Torch_Handle:
	case DecorType::Torch_Bowl:
	default:
		break;
	}

	if (interactable)
		interactable->setType(type);

	return interactable;
}