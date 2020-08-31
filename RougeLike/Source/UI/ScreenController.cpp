#include "pch.h"
#include "ScreenController.h"

#include "UI/UIManager.h"
#include "Screens/PopupScreen.h"


// temp
#include "Game/GameController.h"



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
}


void ScreenController::addScreen(ScreenType type)
{
	Screen* screen = getPoolScreen(type);
	mScreens.addState(screen);
}


void ScreenController::replaceScreen(ScreenType type)
{
#if _DEBUG
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


void ScreenController::openPopup(const XMLNode& textNode)
{
	if (mEnablePopups)
	{
		Screen* screen = mUI->mScreenPool.screenRef(ScreenType::Popup);
		PopupScreen* popup = static_cast<PopupScreen*>(screen);
		popup->setMainText(textNode.value());

		addScreen(ScreenType::Popup);
		addSystemState(SystemStates::PauseState);
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