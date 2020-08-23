#pragma once

#include "Input/Button.h"

class Ability;
class UIElement;
class UIBox;
class UITextBox;
enum class AbilityState;
class Texture;


using Elements = std::vector<UIElement*>;


class HotKey
{
public:
	HotKey(Ability* ability, int count);

	Elements getComponents();
	void updateButtonState(std::unordered_map<AbilityState, Texture*>& buttonStates);

private:
	UIBox* creatIconBG() const;
	UIBox* createIcon(const UIBox* iconBg) const;
	UIBox* createButton(const UIBox* iconBG) const;
	UITextBox* createButtonText(UIBox* button, const BasicString& text);


public:
	const Ability* mAbility;

	int mID;
	Button::Key mKey;


private:
	UIBox* mIcon;
	UIBox* mIconBG;
	UITextBox* mKeyText;
	UIBox* mButton;
};