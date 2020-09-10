#include "pch.h"
#include "PropertyMap.h"


PropertyMap::PropertyMap(XMLNode node)
{
	if (node)
	{
		fill(node);
	}
	else
	{
		DebugPrint(Warning, "No property node provided, cannot build propertyMap\n");
	}
	
}


void PropertyMap::fill(const XMLNode& node)
{
	StringMap map(node);
	for (StringMap::const_iterator iter = map.begin(); iter != map.end(); iter++)
	{
		PropertyType name(iter->first);
		float value = (float)atof(iter->second.c_str());
		mData[name] = value;
	}
}

void PropertyMap::addXYPosition(VectorF positon)
{
	mData[PropertyType::xPosition] = positon.x;
	mData[PropertyType::yPosition] = positon.y;
}