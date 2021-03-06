#pragma once

#include "States/StateMachine.h"
#include "ScreenTypes.h"

class GameController;
class UIManager;
class Screen;
enum class SystemStates;


// Couples the UI to the Game
class ScreenController
{
public:
	ScreenController();

	void init(GameController* gameController, UIManager* UI);

	void openPopup(const BasicString& textFile);
	
	void addScreen(ScreenType type);
	void replaceScreen(ScreenType type);

	void processScreenChanges();
	void popScreen();

	int screenCount() const { return mScreens.size(); }
	void clearScreenStack();

	Screen* getActiveScreen() const { return &mScreens.getActiveState(); }

	void quitGame();
	void restartGame();

	void replaceSystemState(SystemStates state);
	void addSystemState(SystemStates state);
	void popSystemState();

	UIManager* ui() { return mUI; }

	// TODO: make const(), need render() to be const first
	void setPersistingScreen(Screen* screen) { mPersistingScreen = screen; }
	Screen* persistingScreen() { return mPersistingScreen; }

	void enablePopups(bool enable) { mEnablePopups = enable; }

private:
	Screen* getPoolScreen(ScreenType type);


private:
	GameController* mGameController;
	UIManager* mUI;

	StateMachine<Screen> mScreens;
	Screen* mPersistingScreen;

	bool mEnablePopups;
};