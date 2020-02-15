#include "pch.h"
#include "XMLParser.h"


Attributes XMLParser::getAttributes(xmlNode node)
{
	ASSERT(Warning, node != nullptr, "Attempting to get attributes for non-existant node\n");
	//DebugPrint(Log, "Getting attributes for node %s\n", node->name());
	Attributes attributes;

	for (xmlAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		attributes.add(attr->name(), attr->value());
	}

	ASSERT(Warning, attributes.getMap().size() != 0, "Node %s has no attributes\n", node->name());
	return attributes;
}


std::string XMLParser::getNodeData(xmlNode dataNode)
{
	ASSERT(Warning, dataNode->value() != NULL, "Node %s has no data\n", dataNode->name());
	return std::string(dataNode->value());
}

xmlNode XMLParser::getRootNode()
{
	return xmlFile.first_node();
}
