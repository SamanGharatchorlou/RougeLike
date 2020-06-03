#include "pch.h"
#include "AbilityFinder.h"


#include "Objects/Abilities/SlowAbility.h"
#include "Objects/Abilities/HealAbility.h"
#include "Objects/Abilities/SpikeAbility.h"
#include "Objects/Abilities/BilnkAbility.h"
#include "Objects/Abilities/ArmorAbility.h"


AbilityFinder::AbilityFinder()
{

}

Ability* AbilityFinder::get(const std::string& ability)
{
	XMLParser parser;


	//if (ability == "Slow")
	//{
	//	return new SlowAbility;
	//}
	//if (ability == "Heal")
	//{
	//	return HealAbility;
	//}
	//if (ability == "Spikes")
	//{
	//	return new SpikeAbility;
	//}
	//if (ability == "Blink")
	//{
	//	return new BlinkAbility;
	//}
	if (ability == "Armor")
	{
		parser.parseXML(FileManager::Get()->XMLFilePath(FileManager::Config_Abilities, ability));
		ValueMap values = parser.values(parser.rootNode());

		return new ArmorAbility(std::stof(values["Power"]));
	}
}