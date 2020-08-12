#include "pch.h"
#include "XMLNode.h""


StringMap XMLNode::attributes() const
{
	StringMap stringMap;
	for (xmlNodeAttributes attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		stringMap[attr->name()] = attr->value();
	}

	return stringMap;
}


//std::unordered_map<BasicString, BasicString> XMLNode::stringMap() const
//{
//	std::unordered_map<BasicString, BasicString> stringDataMap;
//
//	xmlNodePtr childNode = node->first_node();
//	while (childNode != nullptr)
//	{
//		stringDataMap[childNode->name()] = childNode->value();
//		childNode = childNode->next_sibling();
//	}
//
//	if(stringDataMap.empty())
//		DebugPrint(Log, "Node %s has no child values\n", node->name());
//
//	return stringDataMap;
//}
//
//
//DataMap<BasicString, float> XMLNode::floatMap() const
//{
//	DataMap<float> valueMap;
//
//	xmlNodePtr childNode = node->first_node();
//	while (childNode != nullptr)
//	{
//		valueMap[childNode->name()] = atof(childNode->value());
//		childNode = childNode->next_sibling();
//	}
//
//	if (!valueMap.empty())
//		DebugPrint(Log, "Node %s has no child values\n", node->name());
//
//	return valueMap;
//}

//float XMLNode::getFloat() const
//{
//	return atof(node->value());
//}
//
//
//int XMLNode::getInt() const
//{
//	return atoi(node->value());
//}




//VectorF getXYAttributes(XMLNode node)
//{
//	//StringMap attributes = node.nodeAttributes();
//	//float x = attributes.getFloat("x");
//	//float y = attributes.getFloat("y");
//	return VectorF(0, 0);
//}