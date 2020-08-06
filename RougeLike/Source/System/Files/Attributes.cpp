#include "pch.h"
#include "Attributes.h"

void Attributes::set(StringMap stringMap)
{
	attributes = stringMap;
}


void Attributes::merge(Attributes otherAttributes)
{
	attributes.insert(otherAttributes.getMap().begin(), otherAttributes.getMap().end());
}


void Attributes::add(const BasicString& name, BasicString value)
{
	attributes[name] = value;
}


bool Attributes::contains(const BasicString& name) const
{
	return (bool)attributes.count(name);
}


const BasicString& Attributes::getString(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	//BasicString value = attributes[name];
	return attributes.at(name);
}


const int Attributes::getInt(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	return atoi(attributes.at(name).c_str());
}


const int Attributes::getBool(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	return attributes.at(name) == "true";
}


const float Attributes::getFloat(const BasicString& name) const
{
#if _DEBUG
	if (!contains(name))
		DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
	return atof(attributes.at(name).c_str());
}


void Attributes::log() const
{
	DebugPrint(Log, "\nLoading level atrributes...\n");
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		DebugPrint(Log, "Atrribute %s has value %s\n", iter->first.c_str(), iter->second.c_str());
	}
	DebugPrint(Log, "\n");
}
