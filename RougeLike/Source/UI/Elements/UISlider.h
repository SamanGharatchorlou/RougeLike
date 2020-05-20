#pragma once

class UIManager;
class UIButton;
class UIElement;

class UISlider
{
public:
	UISlider();
	void setComponents(UIButton* slider, UIElement* bar, UIElement* container);

	void setCursorOffset(float cursorPosition);

	void setPosition(float position);
	void setValue(float value);

	void setActive(bool active);
	bool isActive() const;
	bool isPressed() const;

	float value() const;

private:
	UIButton* mSlider;
	UIElement* mBar;
	UIElement* mContainer;

	float xCursorOffset;
};