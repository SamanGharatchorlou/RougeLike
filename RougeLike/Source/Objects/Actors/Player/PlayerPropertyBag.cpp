#include "pch.h"
#include "PlayerPropertyBag.h"

#include "Objects/Attributes/Level.h"
#include "Objects/Attributes/Armor.h"

void PlayerPropertyBag::readProperties(XMLParser& parser)
{
	ValueMap map = readValues(parser);
	fillProperties(map);
}


void PlayerPropertyBag::fillProperties(ValueMap& valueMap)
{
	for (ValueMap::iterator iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		std::string name = iter->first;

		if (name == "Level")
		{
			Level* level = new Level;
			mProperties[name] = level;

			// Set has been visited bool to true
			iter->second.second = true;
		}

		else if (name == "Armor")
		{
			Armor* armor = new Armor(iter->second.first);
			mProperties[name] = armor;

			// Set has been visited bool to true
			iter->second.second = true;
		}
	}

	PropertyBag::fillProperties(valueMap);
}