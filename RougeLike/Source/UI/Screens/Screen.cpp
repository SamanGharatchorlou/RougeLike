#include "pch.h"
#include "Screen.h"

#include "UI/ScreenLayers.h"

#include "UI/Elements/UISwitch.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISlider.h"


Screen::~Screen()
{
	Elements elements = mScreenLayers.elementList();

	for (UIElement*& element : elements)
	{
		delete element;
		element = nullptr;
	}

	mSliders.clear();
	mButtons.clear();
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


void Screen::linkButton(ScreenItem option, const BasicString& buttonId)
{
	UIButton* button = findButton(buttonId);

#if _DEBUG
	if (!button)
	{
		DebugPrint(Warning, "No button element '%s' found on screen type %d\n", buttonId, type());
		return;
	}
#endif
	mButtons[option] = button;
}


void Screen::linkSwitch(ScreenItem option, const BasicString& switchId)
{
	UIElement* element = find(switchId);

#if _DEBUG
	if (!element)
	{
		DebugPrint(Warning, "No element '%s' found on screen type %d\n", switchId, type());
		return;
	}

	if (element->type() != UIElement::Type::Switch)
	{
		DebugPrint(Warning, "Element '%s' is not a switch type\n", switchId);
		return;
	}
#endif


	UISwitch* uiSwitch = static_cast<UISwitch*>(element);
	mSwitches[option] = uiSwitch;
}


void Screen::linkSlider(ScreenItem option, const BasicString& sliderId)
{
	UIElement* element = find(sliderId);

#if _DEBUG
	if (!element)
	{
		DebugPrint(Warning, "No element '%s' found on screen type %d\n", sliderId, type());
		return;
	}

	if (element->type() != UIElement::Type::Slider)
	{
		DebugPrint(Warning, "Element '%s' is not a slider type\n", sliderId);
		return;
	}
#endif

	UISlider* slider = static_cast<UISlider*>(element);
	mSliders[option] = slider;
}


bool Screen::selected(ScreenItem item) const
{
#if _DEBUG
	if (mButtons.count(item) == 0)
	{
		DebugPrint(Warning, "No button has been linked to the screen item %d\n", (int)item);
		return false;
	}
#endif
	return mButtons.at(item)->isPressed();
}


UISlider* Screen::slider(ScreenItem item)
{
#if _DEBUG
	if (mSliders.count(item) == 0)
	{
		DebugPrint(Warning, "No slider has been linked to the screen item %d\n", (int)item);
		return nullptr;
	}
#endif
	return mSliders[item];
}