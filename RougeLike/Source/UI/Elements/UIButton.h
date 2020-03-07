#pragma once
#include "UIBox.h"
#include "Graphics/Text.h"

class Texture;

class UIButton : public UIBox
{
public:
	enum Action
	{
		None_0,
		ResumeGame_1,
		QuitGame_2,
		RestartGame_3,

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


// Dont need a text button
// replace with with text box and a button, seperat objects might be better
/*
class UITextButton : public UIButton
{
public:
	struct Data : public UIButton::Data
	{
		std::string text;
		std::string font;
		int ptSize;
		SDL_Color colour;
	};

public:
	UITextButton() { }
	UITextButton(Data& data);

	void render() override;

	virtual bool hasText() const { return true; }
	virtual Type type() const { return Type::TextButton; }

private:
	Text mText;
};
*/