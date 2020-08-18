#pragma once

#include "ScreenTypes.h"

class UIButton;
class UIElement;
class UISlider;

class InputManager;

enum class ScreenItem
{
	None,

	// Character Selection
	Play,

	// Game
	Health,
	Armor,

	// Pause
	Resume,
	Settings,
	Restart,
	Quit,

	// Settings
	Music,
	Sound,
	Mute,
	Close,
	Count
};


class Screen
{
public:
	enum Type
	{
		None,
		CharacterSelection,
		Game,
		Pause,
		Settings
	};


public:
	virtual ~Screen();

	void add(const ScreenLayer& layer);
	void add(const ScreenLayers& layers);

	ScreenLayers& layers() { return mScreenLayers; }
	ScreenLayer* layer(const BasicString& id);

	void updateButtons(const InputManager* input);

	virtual void enter() = 0;
	virtual void update(float dt) = 0;
	virtual void exit() = 0;
	virtual void render();

	virtual Type type() = 0;

	UIElement* find(const BasicString& id);
	UIButton* findButton(const BasicString& id);

	void linkSlider(ScreenItem setting, const BasicString& sliderId);
	void linkButton(ScreenItem option, const BasicString& buttonId);
	bool selected(ScreenItem button) const;

protected:
	ScreenLayers mScreenLayers;

	std::unordered_map<ScreenItem, UISlider*> mSliders;
	std::unordered_map<ScreenItem, UIButton*> mButtons;
};



class NullScreen : public Screen
{
public:
	void enter() { }
	void handleInput(const InputManager* input) { }
	void update(float dt) { }
	void exit() { }
	void render() { }
	Type type() { return Type::None; }
};