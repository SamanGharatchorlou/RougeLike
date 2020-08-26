#include "pch.h"
#include "ScreenLayers.h"

#include "UI/Elements/UIElement.h"



Elements ScreenLayers::elementList() const
{
	Elements elements;

	for (const ScreenLayer& screenLayer : mLayers)
	{
		elements.insert(elements.end(), screenLayer.elements.begin(), screenLayer.elements.end());
	}

	return elements;
}


void ScreenLayers::add(ScreenLayer layer)
{
	mLayers.push_back(layer);
}


UIElement* ScreenLayers::find(const BasicString& id)
{
	Elements elements = elementList();

	for (UIElement* element : elements)
	{
		if (id == element->id())
			return element;
	}

	DebugPrint(Warning, "No element with the id %s was found from the provided screen\n", id.c_str());
	return nullptr;
}


ScreenLayer* ScreenLayers::layer(const BasicString& id)
{
	for (int i = 0; i < mLayers.size(); i++)
	{
		if (mLayers[i].id == id)
			return &mLayers[i];
	}

	DebugPrint(Warning, "No layer with the id '%s' exists, returning layer at index 0\n");
	return nullptr;
}