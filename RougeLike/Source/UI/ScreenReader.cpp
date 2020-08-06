#include "pch.h"
#include "ScreenReader.h"



const ScreenAttributes ScreenReader::readScreen(const BasicString& screen) const
{
	ScreenAttributes screenAttributes;

	XMLParser parser(screen);
	XMLNode screenNode = parser.root();
	XMLNode layerNode = screenNode.first("layer");

	while (!layerNode.isEmpty())
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

	XMLNode itemNode = layerNode.first();

	// UI component
	while (!itemNode.isEmpty())
	{
		Attributes itemAttributes = readItemNode(itemNode);
		layerAttributes.push_back(itemAttributes);

		itemNode = itemNode.next();
	}

	return layerAttributes;
}


Attributes ScreenReader::readItemNode(const XMLNode itemNode) const
{
	Attributes itemAttributes = itemNode.attributes();
	itemAttributes.add("type", itemNode.name());

	if (isTextNode(itemNode))
	{
		Attributes textAttributes = readtextNode(itemNode);
		itemAttributes.merge(textAttributes);
	}

	return itemAttributes;
}


Attributes ScreenReader::readtextNode(const XMLNode itemNode) const
{
	Attributes textAttributes;
	XMLNode textNode = itemNode.first();
	if (textNode)
	{
		textAttributes = textNode.attributes();

		// text to be displayed
		BasicString text = textNode.value();
		textAttributes.add("text", text);

		textAttributes.merge(textAttributes);
	}

	return textAttributes;
}


bool ScreenReader::isTextNode(const XMLNode itemNode) const
{
	return itemNode.name() == "TextBox" && !itemNode.first().isEmpty();
}