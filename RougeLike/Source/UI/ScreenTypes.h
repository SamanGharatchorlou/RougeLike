#pragma once

#include "Utilities/BasicString.h"
#if _DEBUG
#include "Debug/Logging.h"
#endif


enum class ScreenType
{
	// Pre loading
	LoadingScreen,

	None,
	CharacterSelection,
	Game,
	Pause,
	Settings,
	Popup,
	Count
};


// Type to string
inline void operator >>(ScreenType type, BasicString& str)
{
	if (type == ScreenType::LoadingScreen)
		str = "LoadingScreen";
	else if (type == ScreenType::CharacterSelection)
		str = "CharacterSelectionScreen";
	else if (type == ScreenType::Game)
		str = "GameScreen";
	else if (type == ScreenType::Pause)
		str = "PauseScreen";
	else if (type == ScreenType::Settings)
		str = "SettingsScreen";
	else if (type == ScreenType::Popup)
		str = "PopupScreen";
	else
	{
		str.clear();
		DebugPrint(Warning, "No screen type to string conversion defined for screen type %d\n", type);
	}
}


template<class T>
inline ScreenType operator +(ScreenType a, T b)
{
	int sum = static_cast<int>(a) + static_cast<int>(b);
	return static_cast<ScreenType>(sum);
}



enum class ScreenItem
{
	None,

	// Loading
	LoadingBar,

	// Character Selection
	Play,
	Tutorial,
	LeftArrow,
	RightArrow,

	// Game
	Health,
	Exp,
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