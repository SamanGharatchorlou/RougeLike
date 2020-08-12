#include "pch.h"
#include "ScreenReader.h"

/*
HI samoyed SAUSAGE 
*/

const ScreenAttributes ScreenReader::readScreen(const BasicString& screen) const
{
	ScreenAttributes screenAttributes;

	XMLParser parser(screen);
	XMLNode layerNode = parser.rootChild("Layer");
	while (layerNode)
	{
		LayerAttributes layerAttributes = readLayer(layerNode);
		screenAttributes.push_back(layerAttributes);

		layerNode = layerNode.next();
	}

	return screenAttributes;
}


LayerAttributes ScreenReader::readLayer(const XMLNode layerNode) const
{
	LayerAttributes layerAttributes;

	XMLNode itemNode = layerNode.child();
	while (itemNode)
	{
		StringMap itemAttributes = readItemNode(itemNode);
		layerAttributes.push_back(itemAttributes);

		itemNode = itemNode.next();
	}

	return layerAttributes;
}


StringMap ScreenReader::readItemNode(const XMLNode itemNode) const
{
	StringMap itemAttributes = itemNode.attributes();
	itemAttributes["type"] = itemNode.name();

	if (isTextNode(itemNode))
	{
		StringMap textAttributes = readtextNode(itemNode);
		itemAttributes.merge(textAttributes);
	}

	return itemAttributes;
}


StringMap ScreenReader::readtextNode(const XMLNode itemNode) const
{
	StringMap textAttributes;
	XMLNode textNode = itemNode.child();
	if (textNode)
	{
		textAttributes = textNode.attributes();

		// text to be displayed
		BasicString text = textNode.value();
		textAttributes["text"] = text;
	}

	return textAttributes;
}


bool ScreenReader::isTextNode(const XMLNode itemNode) const
{
	return itemNode.name() == "TextBox" && itemNode.child();
}