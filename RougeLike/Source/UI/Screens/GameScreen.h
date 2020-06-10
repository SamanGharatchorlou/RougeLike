#pragma once

#include "Screen.h"
#include "UI/Elements/UIBox.h"

struct GameData;

class GameScreen : public Screen
{
public:
	GameScreen(GameData* gameData);

	void enter() override {}
	void update(float dt) override { }
	void render() override;
	void exit() override { }

	Type type() override { return Type::Game; }

	void AddBox(UIBox box) { mAbilityHotKeys.push_back(box); }

private:

	std::vector<UIBox> mAbilityHotKeys;
};