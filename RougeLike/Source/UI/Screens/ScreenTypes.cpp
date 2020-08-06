#include "pch.h"
#include "ScreenTypes.h"

#include "UI/Elements/UIElement.h"



Elements ScreenLayers::elementList() const
{
	Elements elements;

	for (const ScreenLayer& screenLayer : mLayers)
	{
		elements.insert(elements.end(), screenLayer.begin(), screenLayer.end());
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
