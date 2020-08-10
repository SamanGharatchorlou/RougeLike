#include "pch.h"
#include "XMLNode.h""


DataMap<BasicString> XMLNode::nodeAttributes() const
{
	DataMap<BasicString> attributes;
	for (xmlNodeAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		attributes.add(attr->name(), attr->value());
	}

	return attributes;
}


DataMap<BasicString> XMLNode::stringMap() const
{
	DataMap<BasicString> stringDataMap;

	xmlNodePtr childNode = node->first_node();
	while (childNode != nullptr)
	{
		stringDataMap[childNode->name()] = childNode->value();
		childNode = childNode->next_sibling();
	}

	if(stringDataMap.empty())
		DebugPrint(Log, "Node %s has no child values\n", node->name());

	return stringDataMap;
}

DataMap<float> XMLNode::floatMap() const
{
	DataMap<float> valueMap;

	xmlNodePtr childNode = node->first_node();
	while (childNode != nullptr)
	{
		valueMap[childNode->name()] = atof(childNode->value());
		childNode = childNode->next_sibling();
	}

	if (!valueMap.empty())
		DebugPrint(Log, "Node %s has no child values\n", node->name());

	return valueMap;
}

float XMLNode::getFloat() const
{
	return atof(node->value());
}


int XMLNode::getInt() const
{
	return atoi(node->value());
}




VectorF getXYAttributes(XMLNode node)
{
	StringMap attributes = node.nodeAttributes();
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");
	return VectorF(x, y);
}