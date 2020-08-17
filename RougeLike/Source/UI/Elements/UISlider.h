#pragma once

#include "UIBox.h"

class UIButton;
class InputManager;


class UISlider : public UIBox
{
public:
	UISlider(const StringMap& attributes);

	void setComponents(Texture* container, UIButton* slider, UIBox* bar);


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
};

