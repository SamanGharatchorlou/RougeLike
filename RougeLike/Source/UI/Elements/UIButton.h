#pragma once
#include "UIBox.h"

class Texture;

class UIButton : public UIBox
{
public:
	enum Action
	{
		None_0,

		// Pause Screen
		ResumeGame_1,
		QuitGame_2,
		RestartGame_3,

		// Character Selection Screen
		SelectSoldier_4,

		COUNT
	};

	struct Data : public UIBox::Data
	{
		// default texture from UIBox is used for the default
		Texture* highlightedTexture;
		Action action;
	};


public:
	UIButton();
	UIButton(Data& data);
	~UIButton() { }

	void setPressed(bool isPressed);
	void setReleased(bool isReleased);

	inline bool isPressed() const { return mIsPressed; }
	inline bool isReleased() const { return mIsReleased; }

	const Action getAction() const { return mAction; }

	virtual bool isButton() const { return true; }
	virtual bool hasText() const { return false; }
	virtual Type type() const { return Type::Button; }

private:
	Texture* mDefault;
	Texture* mHighlighted;

	Action mAction;	
	
	bool mIsPressed;
	bool mIsReleased;
};
