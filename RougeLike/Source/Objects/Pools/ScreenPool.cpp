#include "pch.h"
#include "ScreenPool.h"

#include "UI/Screens/ScreenHeaders.h"
#include "UI/ScreenBuilder.h"


Screen* ScreenPool::createNewObjects(ScreenType type, int count, int& outSize) const
{
	Screen* screen = nullptr;

	switch (type)
	{
	case ScreenType::LoadingScreen:
		screen = new LoadingScreen[count];
		outSize = sizeof(LoadingScreen);
		break;
	case ScreenType::CharacterSelection:
		screen = new CharacterSelectionScreen[count];
		outSize = sizeof(CharacterSelectionScreen);
		break;
	case ScreenType::Game:
		screen = new GameScreen[count];
		outSize = sizeof(GameScreen);
		break;	
	case ScreenType::GameOver:
		screen = new GameOverScreen[count];
		outSize = sizeof(GameOverScreen);
		break;
	case ScreenType::Pause:
		screen = new PauseScreen[count];
		outSize = sizeof(PauseScreen);
		break;
	case ScreenType::Settings:
		screen = new SettingsScreen[count];
		outSize = sizeof(SettingsScreen);
		break;
	case ScreenType::Popup:
		screen = new PopupScreen[count];
		outSize = sizeof(PopupScreen);
		break;	
	case ScreenType::None:
	default:
		DebugPrint(Warning, "Invalid screen type %d, could not create %d objects\n", type, count);
		break;
	}

	// MOVE THIS? - doesnt really belong here... move into screen controller class
	const ScreenBuilder builder;
	for (int i = 0; i < count; i++)
	{
		Screen* screenPtr = screen + i;
		builder.populateScreen(screenPtr);
	}

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