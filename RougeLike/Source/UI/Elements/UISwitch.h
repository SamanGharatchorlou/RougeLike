#pragma once

#include "UIBox.h"
#include "Input/Button.h"


class Texture;

class UISwitch : public UIBox
{
public:
	enum State
	{
		Off,
		On
	};


	struct Data : public UIBox::Data
	{
		Texture* on = nullptr;
		Texture* off = nullptr;
	};

public:
	UISwitch(const StringMap& attributes);

	void setTextures(Texture* off, Texture* on);

	void reset() { }
	void handleInput(const InputManager* input) override;


	void setState(State state);
	void switchState();
	State state() const { return mState; }

	virtual Type type() const { return Type::Switch; }


private:
	Texture* mOn;
	Texture* mOff;

	State mState;
};