#include "pch.h"
#include "XMLParser.h"


Attributes XMLParser::attributes(xmlNode node) const
{
	ASSERT(Warning, node != nullptr, "Attempting to get attributes for non-existant node\n");
	Attributes attributes;

	for (xmlAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		attributes.add(attr->name(), attr->value());
	}

	ASSERT(Warning, attributes.getMap().size() != 0, "Node %s has no attributes\n", node->name());
	return attributes;
}


std::string XMLParser::firstRootNodeValue(const std::string& label) const
{
	xmlNode node = rootNode()->first_node(label.c_str());
	
	ASSERT(Warning, node != nullptr, "The node '%s' does not exist, it must have a value\n", label.c_str());
	return node->value();
}

xmlNode XMLParser::rootNode() const
{
	return xmlFile.first_node();
}
