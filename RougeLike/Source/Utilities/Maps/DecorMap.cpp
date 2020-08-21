#include "pch.h"
#include "DecorMap.h"

#include "Map/Tiles/TileTypes.h"


void DecorMap::fill(const XMLNode& node)
{
	XMLNode decorNode = node.child();
	while (decorNode)
	{
		DecorType type = stringToDecorType(decorNode.name());
		StringMap attributes(decorNode.attributes());

		mData[type] = attributes;

		decorNode = decorNode.next();
	}
}