#include "pch.h"
#include "Screen.h"

#include "UI/Elements/UIElement.h"
#include "UI/Elements/UIButton.h"


Screen::~Screen()
{
	Elements elements = mScreenLayers.elementList();

	for (UIElement*& element : elements)
	{
		delete element;
		element = nullptr;
	}
}


void Screen::add(const ScreenLayer& layer)
{
	mScreenLayers.add(layer);
}

void Screen::add(const ScreenLayers& layers)
{
	for (int i = 0; i < layers.size(); i++)
	{
		mScreenLayers.add(layers.layer(i));
	}
}


ScreenLayer* Screen::layer(const BasicString& id)
{
	return mScreenLayers.layer(id);
}

// TODO: change to handle input
void Screen::updateButtons(const InputManager* input)
{
	Elements elements = mScreenLayers.elementList();
	for (UIElement* element : elements)
	{
		element->handleInput(input);
	}
}


void Screen::render()
{
	for (const ScreenLayer& layer : mScreenLayers.layers())
	{
		for (UIElement* element : layer.elements)
		{
			element->render();
		}
	}
}



UIElement* Screen::find(const BasicString& id)
{
	return mScreenLayers.find(id);
}

UIButton* Screen::findButton(const BasicString& id)
{
	UIElement* element = mScreenLayers.find(id);

	if (element->type() == UIElement::Type::Button)
	{
		UIButton* button = static_cast<UIButton*>(element);
		return button;
	}
	else
	{
		DebugPrint(Log, "Attemping to use findButton on a none button element, ID: '%s'\n", id.c_str());
		return nullptr;
	}
}