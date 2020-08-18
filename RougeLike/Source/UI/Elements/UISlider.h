#pragma once

#include "UIBox.h"

class UIButton;
class InputManager;


class UISlider : public UIBox
{
public:
	UISlider(const StringMap& attributes);
	virtual ~UISlider();

	void setComponents(Texture* container, UIButton* slider, UIBox* bar);
	void disableInput() { mSelectable = false; }

	void handleInput(const InputManager* input) override;

	void setCursorOffset(float cursorPosition);

	void setSliderPosition(float position);
	void setSliderValue(float value);
	
	float getValue() const;

	virtual void render() override;

	UIButton* handle() { return mSlider; }

	virtual Type type() const { return Type::Slider; }


private:
	void updateBar();

	void setBarRect(float value);
	void setBarSubRect(float value);


private:
	UIButton* mSlider;
	UIBox* mBar;

	RectF subRect;

	float xCursorOffset;
	bool mSelectable;
};

