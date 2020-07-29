#include "pch.h"
#include "CollectableSpawner.h"



CollectablesMap CollectableSpawner::readLevelData(const XMLParser& parser)
{
	CollectablesMap dataMap;
	xmlNode rootNode = parser.rootNode();
	xmlNode collectablesNode = rootNode->first_node("Collectables");

	if (collectablesNode)
	{
		xmlNode collectableNode = collectablesNode->first_node("Collectable");
		while (collectableNode)
		{
			CollectableType type;
			type << collectableNode->name();
			Attributes attributes = parser.attributes(collectableNode);
			dataMap[type] = attributes;

			collectableNode = collectableNode->next_sibling();
		}
	}

	return dataMap;
}


