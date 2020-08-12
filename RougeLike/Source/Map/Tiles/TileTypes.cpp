#include "pch.h"
#include "TileTypes.h"

DecorType stringToType(const BasicString& decorString)
{
	if (decorString == "Column")
		return DecorType::Column;
	else if (decorString == "Water")
		return DecorType::Water;
	else if (decorString == "Torch")
		return DecorType::Torch;
	else if (decorString == "TorchHandle")
		return DecorType::Torch_Handle;
	else if (decorString == "TorchBowl")
		return DecorType::Torch_Bowl;
	else if (decorString == "Spikes")
		return DecorType::Spikes;	
	else if (decorString == "Trigger")
		return DecorType::Trigger;
	else if (decorString == "Grating")
		return DecorType::Grating;
	else
	{
		DebugPrint(Warning, "No decor tile has been set to the string '%s'\n", decorString.c_str());
		return DecorType::None;
	}
}