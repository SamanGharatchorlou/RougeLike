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

	// Bar rect/subrect at value = 1.0
	RectF defaultBarSubRect() const;
	RectF defaultBarRect() const;


private:
	UIButton* mSlider;

	UIBox* mBar; 
	// only render part of the texture as the rect decreases in width to prevent it warping
	RectF mBarSubRect;

	float xCursorOffset;
	bool mSelectable;
};

