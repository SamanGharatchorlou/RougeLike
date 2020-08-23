#include "pch.h"
#include "XMLNode.h"


StringMap XMLNode::attributes() const
{
	StringMap stringMap;
	for (xmlNodeAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		stringMap[attr->name()] = attr->value();
	}

	return stringMap;
}