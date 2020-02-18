#pragma once

#include "UI/Elements/UIButton.h"
#include "UI/Elements/UIElement.h"


class UILayer
{
public:
	~UILayer();

	void render();

	std::queue<UIButton::Action> onPress(VectorF point);

	void addElement(UIElement* element);

	std::vector<UIElement*> elements() const { return mElements; }

private:
	std::vector<UIElement*> mElements;
};