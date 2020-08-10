#include "pch.h"
#include "Devil.h"

Devil::Devil() { }


void Devil::init(TextureManager* textureManager)
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, "Devil"));
	setCharacter(parser.rootNode(), textureManager);
}