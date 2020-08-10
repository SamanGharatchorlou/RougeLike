#include "pch.h"
//#include "StringMap.h"

/*
void StringMap::set(StringMap stringMap)
{
	attributes = stringMap;
}


void StringMap::merge(StringMap otherAttributes)
{
	attributes.insert(otherAttributes.getMap().begin(), otherAttributes.getMap().end());
}


void StringMap::add(const BasicString& name, BasicString value)
{
	attributes[name] = value;
}


bool StringMap::contains(const BasicString& name) const
{
	return (bool)attributes.count(name);
}


const BasicString& StringMap::getString(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	return attributes.at(name);
}


const int StringMap::getInt(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	return atoi(attributes.at(name).c_str());
}


const int StringMap::getBool(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	return attributes.at(name) == "true";
}


const float StringMap::getFloat(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	return atof(attributes.at(name).c_str());
}


void StringMap::log() const
{
	DebugPrint(Log, "\nLoading level atrributes...\n");
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		DebugPrint(Log, "Atrribute %s has value %s\n", iter->first.c_str(), iter->second.c_str());
	}
	DebugPrint(Log, "\n");
}
*/