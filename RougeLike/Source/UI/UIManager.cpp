#include "pch.h"
#include "UIManager.h"

#include "MenuDecoder.h"
#include "Game/GameData.h"


UIManager::UIManager(GameData* gameData) : 
	pauseMenu(gameData), gameMenu(gameData), activeMenu(&gameMenu) { }


void UIManager::selectMenu(Screen menuScreen)
{
	activeMenu->exit();

	switch (menuScreen)
	{
	//case UIManager::start:
	//	break;
	case UIManager::pause:
		activeMenu = &pauseMenu;
		break;
	case UIManager::game:
		activeMenu = &gameMenu;
		break;
	}

	activeMenu->enter();
}



void UIManager::init()
{
	MenuDecoder menuDecoder;

	fs::path menuDirectoryPath = fs::current_path();
	menuDirectoryPath /= "Resources/Configs/UIMenus";
	ASSERT(Warning, fs::is_directory(menuDirectoryPath), "Path: %s is not a directory\n", menuDirectoryPath.string().c_str());

	fs::path menuPath;
	MenuAttributes attributes;

	// Pause Menu
	menuPath = menuDirectoryPath / "PauseMenu.xml";
	ASSERT(Warning, menuPath.has_filename(), "File: %s does not exist\n", menuPath.string().c_str());

	attributes = menuDecoder.getMenuAttributes(menuPath.string());
	pauseMenu.init(attributes);

	// Game Menu
	menuPath = menuDirectoryPath / "GameMenu.xml";	
	ASSERT(Warning, menuPath.has_filename(), "File: %s does not exist\n", menuPath.string().c_str());

	attributes = menuDecoder.getMenuAttributes(menuPath.string());
	gameMenu.init(attributes);
}