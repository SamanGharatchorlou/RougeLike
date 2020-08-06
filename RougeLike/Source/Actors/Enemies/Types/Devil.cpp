#include "pch.h"
#include "Devil.h"

#include "Objects/Properties/PropertyBag.h"

Devil::Devil() { }


void Devil::init(TextureManager* textureManager)
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, "Devil"));

	setCharacter(parser, textureManager);

	//ValueBag effectValues;
	//effectValues.readData(parser, "Effects");
}