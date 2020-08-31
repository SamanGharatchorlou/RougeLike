#pragma once

#if UI_EDITOR
#include "Input/Button.h"

class Screen;
class InputManager;


class UIEditor
{
public:
	void setScreen(Screen* activeScreen);
	void handleInput(const InputManager* input);

	BasicString selectElement(const InputManager* input);
	VectorF moveElement(const InputManager* input, Button::Key key, VectorF direction);

	void handleEvent(const InputManager* input,VectorF movement);

	void render();

private:
	void printPosition();


private:
	Screen* mScreen;
	BasicString mElementID;

	float movementSpeed = 0.5f; // pixels moved per press
	int holdSpeed = 2; // larger is slower, using % for it
	int holdDelay = 15;
};
#endif