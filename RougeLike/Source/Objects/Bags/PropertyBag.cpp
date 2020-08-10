#include "pch.h"
#include "PropertyBag.h"


PropertyBag::PropertyBag(XMLNode node)
{
	StringMap data = readValues(node);
	fillData(data);
}



PropertyBag::PropertyBag(const FloatMap& valueMap)
{
	FloatMap::const_iterator iter;
	for (iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		mData[iter->first] = iter->second;
	}
}


float PropertyBag::get(PropertyType property) const
{
	if (mData.count(property))
	{
		return mData.at(property);
	}
	else
	{
		DebugPrint(Warning, "Property bag does not contain value with label '%s'\n", property.string().c_str());
		return -1.0f;
	}
}

void PropertyBag::fillData(const StringMap& stringMap)
{
	for (StringMap::const_iterator iter = stringMap.begin(); iter != stringMap.end(); iter++)
	{
		const BasicString name(iter->first);
		float value = atof(iter->second.c_str());
		mData[name] = value;
	}
}

