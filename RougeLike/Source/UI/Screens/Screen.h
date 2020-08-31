#pragma once

#include "States/State.h"
#include "UI/ScreenTypes.h"
#include "UI/ScreenLayers.h"

class UIButton;
class UIElement;
class UISlider;
class UISwitch;
class ScreenController;
class InputManager;



class Screen : public State
{
public:
	Screen() { }
	virtual ~Screen();

	void add(const ScreenLayer& layer);
	void add(const ScreenLayers& layers);

	ScreenLayers& layers() { return mScreenLayers; }
	ScreenLayer* layer(const BasicString& id);

	void updateButtons(const InputManager* input);

	// Unused state override functions
	void handleInput() override { }
	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) { };

	virtual void handleInput(const InputManager* input) { }
	virtual void slowUpdate() = 0;
	void render() override;

	virtual ScreenType type() = 0;

	UIElement* find(const BasicString& id);
	UIButton* findButton(const BasicString& id);

	void linkSlider(ScreenItem option, const BasicString& sliderId);
	void linkButton(ScreenItem option, const BasicString& buttonId);
	void linkSwitch(ScreenItem option, const BasicString& switchId);

	virtual bool selected(ScreenItem button) const;
	UISlider* slider(ScreenItem slider);

	void setController(ScreenController* controller) { mController = controller; }
	ScreenController* controller() { return mController; }

protected:
	ScreenController* mController;
	ScreenLayers mScreenLayers;

	std::unordered_map<ScreenItem, UISlider*> mSliders;
	std::unordered_map<ScreenItem, UIButton*> mButtons;
	std::unordered_map<ScreenItem, UISwitch*> mSwitches;
};



class NullScreen : public Screen
{
public:
	NullScreen() { }

	void init() { }
	void slowUpdate() { }
	void exit() { }
	void render() { }
	ScreenType type() { return ScreenType::None; }

	bool selected(ScreenItem button) const override { return false; }
};