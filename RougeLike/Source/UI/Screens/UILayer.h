#pragma once

#include "UI/Elements/UIButton.h"
#include "UI/Elements/UIElement.h"

// TODO: const everything here that you can!
class UILayer
{
public:
	~UILayer();

	void render();

	void addElement(UIElement* element);

	// TODO: make this const
	std::vector<UIElement*> elements() const { return mElements; }
	UIElement* element(unsigned int i) const { return mElements[i]; }

private:
	std::vector<UIElement*> mElements;
};