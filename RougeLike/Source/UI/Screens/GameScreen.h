#pragma once

#include "Screen.h"

class UIElement;

class GameScreen : public Screen
{
public:
	GameScreen(const TextureManager* textures);


	void enter() override {}
	void handleInput(const InputManager* input) { }; // TODO: link to hot keys?
	void update(float dt) override { }
	void exit() override { }

	Type type() override { return Type::Game; }

	void addElement(UIElement* element);

private:
	ScreenLayer* mAdditionalLayer;
};