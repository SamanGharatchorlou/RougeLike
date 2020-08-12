#include "pch.h"
#include "PropertyMap.h"


PropertyMap::PropertyMap(XMLNode node)
{
	fill(node);
}


void PropertyMap::fill(const XMLNode& node)
{
	StringMap map(node);
	for (StringMap::const_iterator iter = map.begin(); iter != map.end(); iter++)
	{
		PropertyType name(iter->first);
		float value = atof(iter->second.c_str());
		mData[name] = value;
	}
}