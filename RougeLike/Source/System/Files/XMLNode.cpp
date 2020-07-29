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