#include "pch.h"
#include "ValueBag.h"


ValueBag::ValueBag(XMLNode node)
{
	StringMap data = readValues(node);
	fillData(data);
}


void ValueBag::fillData(const StringMap& stringMap)
{
	for (StringMap::const_iterator iter = stringMap.begin(); iter != stringMap.end(); iter++)
	{
		BasicString name = iter->first;
		float value = atof(iter->second.c_str());
		mData[name] = value;
	}
}


float ValueBag::get(const BasicString& value) const
{
	if (mData.contains(value))
	{
		return mData.at(value);
	}
	else
	{
		DebugPrint(Warning, "Value bag does not contain value with label '%s'\n", value.c_str());
		return -1.0f;
	}
}