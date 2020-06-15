#include "pch.h"
#include "XMLParser.h"


void XMLParser::parseXML(const std::string& filePath)
{
	ASSERT(fs::exists(filePath), "File path %s does not exist, cannot parse xml file\n", filePath.c_str());

	file = new rapidxml::file<>(filePath.c_str());
	xmlFile.parse<0>(file->data());

#if _DEBUG
	path = filePath;
#endif
}


Attributes XMLParser::attributes(xmlNode node) const
{
	if (node == nullptr)
		printf("pause");

	ASSERT(Warning, node != nullptr, "Attempting to get attributes for non-existant node\n");
	Attributes attributes;

	for (xmlAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		attributes.add(attr->name(), attr->value());
	}

	ASSERT(Warning, attributes.getMap().size() != 0, "Node %s has no attributes\n", node->name());
	return attributes;
}


ValueMap XMLParser::values(xmlNode node) const
{
	if (node == nullptr)
		printf("pause");
	ASSERT(Warning, node != nullptr, "Attempting to get values for non-existant node\n");
	ValueMap valueMap;

	xmlNode childNode = node->first_node();

	while (childNode != nullptr)
	{
		valueMap[childNode->name()] = childNode->value();
		childNode = childNode->next_sibling();
	}

	ASSERT(Warning, valueMap.size() != 0, "Node %s has no value\n", node->name());
	return valueMap;
}


std::string XMLParser::firstRootNodeValue(const std::string& label) const
{
	xmlNode node = rootNode()->first_node(label.c_str());

	if (node == nullptr)
		printf("pasue");
	
	ASSERT(Warning, node != nullptr, "The node '%s' does not exist, it must have a value\n", label.c_str());
	return node->value();
}


xmlNode XMLParser::rootNode() const
{
	return xmlFile.first_node();
}


std::string XMLParser::nodeValue(xmlNode node, const std::string& label)
{		
	return node->first_node(label.c_str())->value();
}