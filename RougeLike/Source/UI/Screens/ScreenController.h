#pragma once

enum class ScreenType
{
	None,
	CharacterSelection,
	Game,
	Pause,
	Settings,
	Popup
};


class ScreenController
{
public:
	void pushScreen(ScreenType type) { mScreens.push(type); }
	void popScreen() { mScreens.pop(); }

	void clearStack() 
	{
		while (mScreens.size())
			mScreens.pop(); 
	}

	ScreenType activeScreen() const { return mScreens.top(); }

private:
	std::stack<ScreenType> mScreens;
};