#include "pch.h"
#include "ScreenPool.h"

#include "UI/Screens/ScreenHeaders.h"
#include "UI/ScreenBuilder.h"


Screen* ScreenPool::createNewObject(ScreenType type) const
{
	Screen* screen = nullptr;

	switch (type)
	{
	case ScreenType::LoadingScreen:
		screen = new LoadingScreen;
		break;
	case ScreenType::CharacterSelection:
		screen = new CharacterSelectionScreen;
		break;
	case ScreenType::Game:
		screen = new GameScreen;
		break;
	case ScreenType::Pause:
		screen = new PauseScreen;
		break;
	case ScreenType::Settings:
		screen = new SettingsScreen;
		break;
	case ScreenType::Popup:
		screen = new PopupScreen;
		break;	
	case ScreenType::None:
	default:
		DebugPrint(Warning, "No create new screen defined for type %d\n", type);
		break;
	}

	ScreenBuilder builder;
	builder.populateScreen(screen);

	return screen;
}


Screen* ScreenPool::screenRef(ScreenType type) const
{
	if (mPool.at(type).size() > 0)
	{
		return mPool.at(type).front();
	}
	else
	{
		DebugPrint(Warning, "No type %d left in the screen pool, is it already in use?\n", type);
		return nullptr;	
	}
}