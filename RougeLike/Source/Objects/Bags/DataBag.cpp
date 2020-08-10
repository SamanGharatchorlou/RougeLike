#include "pch.h"
#include "DataBag.h"


void DataBag::readData(const XMLNode node)
{
	StringMap map = readValues(node);
	fillData(map);
}


// Pull raw config data from file into code
StringMap DataBag::readValues(XMLNode node) const
{
	StringMap stringMap;

	XMLNode valueNode = node.child();
	while (valueNode)
	{
		stringMap[valueNode.name()] = valueNode.value();
		valueNode = valueNode.next();
	}

	return stringMap;
}