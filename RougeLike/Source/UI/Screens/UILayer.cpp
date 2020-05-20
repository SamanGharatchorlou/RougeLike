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
		if(mElements[i]->isVisible())
			mElements[i]->render();
	}
}


void UILayer::addElement(UIElement* element)
{
	mElements.push_back(element);
}