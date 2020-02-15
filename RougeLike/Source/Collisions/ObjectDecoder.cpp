#include "pch.h"
#include "ObjectDecoder.h"
#include "Environment/Wall.h"


void ObjectDecoder::decodeWalls(std::string objectDataFilePath)
{
	// Setup the parser for level data
	xmlParser.parseXML(objectDataFilePath);

	// Map node (root)
	xmlNode mapNode = xmlParser.getRootNode();
	ASSERT(Error, strcmp(mapNode->name(), "map") == 0, "Incorrect attributes node\n");

	xmlNode objectNode = mapNode->first_node("objectgroup");
	ASSERT(Error, objectNode != nullptr, "No object node found\n");

	xmlNode wallNode = objectNode->first_node("object");
	ASSERT(Error, objectNode != nullptr, "Must have at least 1 object node\n");

	while (wallNode != nullptr)
	{
		Attributes wallAttributes = xmlParser.getNodeAttributes(wallNode);
		mWalls.push_back(new Wall(wallAttributes));

		wallNode = wallNode->next_sibling("object");
	}
}
