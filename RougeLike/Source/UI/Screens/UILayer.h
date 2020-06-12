#pragma once


class UIElement;


class UILayer
{
public:
	~UILayer();

	void render();

	void addElement(UIElement* element);

	std::vector<UIElement*> elements() const { return mElements; }
	UIElement* element(unsigned int i) const { return mElements[i]; }

private:
	std::vector<UIElement*> mElements;
};