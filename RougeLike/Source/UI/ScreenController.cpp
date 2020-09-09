#include "pch.h"
#include "ScreenController.h"

#include "UI/UIManager.h"
#include "Screens/PopupScreen.h"


// temp
#include "Game/GameController.h"
#include "Audio/AudioManager.h"



ScreenController::ScreenController() : mGameController(nullptr), mUI(nullptr), mScreens(new NullScreen) { }


void ScreenController::init(GameController* gameController, UIManager* UI)
{
	mGameController = gameController;
	mUI = UI;
}


void ScreenController::processScreenChanges()
{
	Screen* screen = mScreens.processStateChanges();
	if (screen)
		mUI->mScreenPool.returnObject(screen, screen->type());
}


void ScreenController::clearScreenStack()
{
	// Leave the base null state
	while (screenCount() > 1)
	{
		popScreen();
		processScreenChanges();
	}

	mPersistingScreen = nullptr;
}


void ScreenController::addScreen(ScreenType type)
{
	Screen* screen = getPoolScreen(type);
	mScreens.addState(screen);
#if !DISABLE_UI_AUDIO
	AudioManager::Get()->play("CloseMenu", nullptr);
#endif
}


void ScreenController::replaceScreen(ScreenType type)
{
#if DEBUG_CHECK
	if (mScreens.size() <= 1)
	{
		BasicString screenName;
		type >> screenName;
		DebugPrint(Warning, "Cannot replace the Null state with state %s, maybe add instead?\n", screenName.c_str());
		return;
	}
#endif

	Screen* screen = getPoolScreen(type);
	mScreens.replaceState(screen);
}


void ScreenController::popScreen()
{
	mScreens.popState();
#if !DISABLE_UI_AUDIO
	AudioManager::Get()->play("CloseMenu", nullptr);
#endif
}


Screen* ScreenController::getPoolScreen(ScreenType type)
{
	Screen* screen = mUI->mScreenPool.getObject(type);
	if (!screen)
	{
		DebugPrint(Warning, "No screen of type %d in the pool, is it already being used on the stack?\n", type);
		return nullptr;
	}

	screen->setController(this);
	return screen;
}


void ScreenController::openPopup(const BasicString& popupInfo)
{
	if (mEnablePopups)
	{
		Screen* screen = mUI->mScreenPool.screenRef(ScreenType::Popup);

		if (screen)
		{
			PopupScreen* popup = static_cast<PopupScreen*>(screen);

			XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Menus, popupInfo));
			popup->build(parser.rootNode());

			addScreen(ScreenType::Popup);
			addSystemState(SystemStates::PauseState);
		}
		else
		{
			DebugPrint(Warning, "Popup screen already in use, cannot make another\n");
		}
	}
}


void ScreenController::quitGame()
{
	mGameController->quitGame();
}
void ScreenController::restartGame()
{
	mGameController->restartGame();
}


void ScreenController::replaceSystemState(SystemStates state)
{
	mGameController->replaceState(state);
}
void ScreenController::addSystemState(SystemStates state)
{
	mGameController->addState(state);
}
void ScreenController::popSystemState()
{
	mGameController->popState();
}
