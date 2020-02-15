#include "pch.h"
#include "MenuDecoder.h"


MenuAttributes MenuDecoder::getMenuAttributes(std::string menuDataFilePath)
{
	xmlParser.parseXML(menuDataFilePath);

	xmlNode menuNode = xmlParser.getRootNode();
	ASSERT(Error, strcmp(menuNode->name(), "menu") == 0, "Incorrect attributes node\n");

	xmlNode layerNode = menuNode->first_node("layer");
	ASSERT(Error, strcmp(layerNode->name(), "layer") == 0, "Incorrect attributes node\n");

	MenuAttributes menuAttributes;

	// Get all layers
	while (layerNode != nullptr)
	{
		std::vector<Attributes> layerAttibutes;

		xmlNode childNode = layerNode->first_node();
		ASSERT(Error, strcmp(childNode->name(), "item") == 0, "Menu must have at least 1 item\n");

		while (childNode != nullptr)
		{
			Attributes itemAttributes = xmlParser.getAttributes(childNode);
			layerAttibutes.push_back(itemAttributes);

			childNode = childNode->next_sibling();
		}

		menuAttributes.push_back(layerAttibutes);

		layerNode = layerNode->next_sibling();
	}

	return menuAttributes;
}