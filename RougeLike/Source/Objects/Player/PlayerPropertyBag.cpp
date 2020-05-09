#include "pch.h"
#include "PlayerPropertyBag.h"
#include "System/Files/StatReader.h"

#include "Objects/Attributes/Level.h"
#include "Objects/Attributes/Health.h"

void PlayerPropertyBag::readAttributes(const std::string& name)
{
	attributeName = name;

	StatReader statReader;
	ValueMap map = statReader.getStats(FileManager::Config_Player, name);

	PropertyBag::fillProperties(map);
}



void PlayerPropertyBag::resetAttributes()
{
	readAttributes(attributeName);
} 


void PlayerPropertyBag::fillProperties(ValueMap& valueMap)
{

}