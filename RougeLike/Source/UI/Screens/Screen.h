#pragma once

#include "UILayer.h"

struct GameData;

typedef std::vector<Attributes> LayerAttributes;
typedef std::vector<LayerAttributes> ScreenAttributes;


class Screen
{
public:
	enum Type
	{
		Start,
		Pause,
		Game,
		CharacterSelection
	};

public:
	Screen(GameData* gameData);
	~Screen();

	void set(std::vector<UILayer*> layers);
	void handleInput();

	virtual void render();

	virtual void update() = 0;
	virtual void enter() = 0;
	virtual void exit() = 0;

	virtual Type type() = 0;

	std::vector<UILayer*> layers() const { return mLayers; }

protected:
	GameData* mGameData;

	std::vector<UILayer*> mLayers;

	std::queue<UIButton::Action> mActions;
};