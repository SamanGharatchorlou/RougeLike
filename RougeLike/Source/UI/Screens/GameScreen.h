#pragma once

#include "Screen.h"

struct GameData;
class UIElement;

class GameScreen : public Screen
{
public:
	GameScreen(GameData* gameData);


	void enter() override {}
	void update(float dt) override { }
	void exit() override;

	Type type() override { return Type::Game; }

	void addElement(UIElement* element);

private:

	UILayer* mAdditionalLayer;
};