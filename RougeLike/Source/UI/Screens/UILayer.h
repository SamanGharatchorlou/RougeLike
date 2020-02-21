#pragma once

#include "UI/Elements/UIButton.h"
#include "UI/Elements/UIElement.h"


class UILayer
{
public:
	~UILayer();

	void resetButtons();
	void render();

	void onPress(VectorF point);
	std::queue<UIButton::Action> onRelease(VectorF point);

	void addElement(UIElement* element);

	std::vector<UIElement*> elements() const { return mElements; }

private:
	std::vector<UIElement*> mElements;
};