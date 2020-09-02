#include "pch.h"
#include "Devil.h"

Devil::Devil() { }


void Devil::init()
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, "Devil"));

	XMLParser animator(FileManager::Get()->findFile(FileManager::Configs_Objects, "DevilAnim"));
	XMLNode animatorNode = animator.rootChild("Animator");

	setCharacter(parser.rootNode(), animatorNode);
}