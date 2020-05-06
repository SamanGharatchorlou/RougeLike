#pragma once

#include "Screen.h"

struct GameData;

class SettingsScreen : public Screen
{
public:
	SettingsScreen(GameData* gameData);

	void update() override;

	void enter() override { }
	void exit() override { };

	Type type() override { return Type::Settings; }
};