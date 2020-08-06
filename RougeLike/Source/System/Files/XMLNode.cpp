#include "pch.h"
#include "XMLNode.h""


Attributes XMLNode::attributes() const
{
	ASSERT(Warning, node != nullptr, "Attempting to get attributes for non-existant node\n");
	Attributes attributes;

	for (xmlAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		attributes.add(attr->name(), attr->value());
	}

	return attributes;
}


StringMap XMLNode::stringMap() const
{
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

ValueMap XMLNode::valueMap() const
{
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