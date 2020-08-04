#include "pch.h"
#include "XMLParser.h"


void XMLParser::parseXML(const BasicString& filePath)
{
	ASSERT(fs::exists(filePath.c_str()), "File path %s does not exist, cannot parse xml file\n", filePath.c_str());

	file = new rapidxml::file<>(filePath.c_str());
	xmlFile.parse<0>(file->data());

#if _DEBUG
	path = filePath;
	printf("created '%s'\n", path.c_str());
#endif
}

XMLParser::~XMLParser()
{
	printf("deleted '%s'\n", path.c_str());
	delete file;

}


Attributes XMLParser::attributes(xmlNode node) const
{
	ASSERT(Warning, node != nullptr, "Attempting to get attributes for non-existant node\n");
	Attributes attributes;

	for (xmlAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		attributes.add(attr->name(), attr->value());
	}

	return attributes;
}


StringMap XMLParser::stringMap(xmlNode node) const
{
	ASSERT(Warning, node != nullptr, "Attempting to get values for non-existant node\n");
	StringMap stringMap;

	xmlNode childNode = node->first_node();

	while (childNode != nullptr)
	{
		stringMap[childNode->name()] = childNode->value();
		childNode = childNode->next_sibling();
	}

	ASSERT(Warning, stringMap.size() != 0, "Node %s has no value\n", node->name());
	return stringMap;
}

ValueMap XMLParser::valueMap(xmlNode node) const
{
	ASSERT(Warning, node != nullptr, "Attempting to get values for non-existant node\n");
	ValueMap valueMap;

	xmlNode childNode = node->first_node();

	while (childNode != nullptr)
	{
		valueMap[childNode->name()] = atof(childNode->value());
		childNode = childNode->next_sibling();
	}

	ASSERT(Warning, valueMap.size() != 0, "Node %s has no value\n", node->name());
	return valueMap;
}


BasicString XMLParser::firstRootNodeValue(const BasicString& label) const
{
	xmlNode node = rootNode()->first_node(label.c_str());
	
	ASSERT(Warning, node != nullptr, "The node '%s' does not exist, it must have a value\n", label.c_str());
	return node->value();
}


xmlNode XMLParser::rootNode() const
{
	return xmlFile.first_node();
}

XMLNode XMLParser::root() const
{
	return XMLNode(xmlFile.first_node());
}


BasicString XMLParser::nodeValue(xmlNode node, const BasicString& label) const
{		
	return node->first_node(label.c_str())->value();
}