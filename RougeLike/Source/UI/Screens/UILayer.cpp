#include "pch.h"
#include "UILayer.h"


UILayer::~UILayer()
{
	for (unsigned int i = 0; i < mElements.size(); i++)
	{
		delete mElements[i];
	}

	mElements.clear();
}


void UILayer::render()
{
	for (unsigned int i = 0; i < mElements.size(); i++)
	{
		mElements[i]->render();
	}
}


void UILayer::addElement(UIElement* element)
{
	mElements.push_back(element);
}


// TODO: change this to release, or make a onRelease version
std::queue<UIButton::Action> UILayer::onPress(VectorF point)
{
	std::queue<UIButton::Action> actions;

	for (UIElement* element : mElements)
	{
		if (element->isButton())
		{
			UIButton* button = static_cast<UIButton*>(element);

			if (button->isPointInBounds(point))
			{
				button->setPressed(true);
				actions.push(button->getAction());
			}
		}
	}

	return actions;
}