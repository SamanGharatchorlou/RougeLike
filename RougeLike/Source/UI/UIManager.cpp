#include "pch.h"
#include "UIManager.h"

#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"

#include "UIEventHandler.h"

#include "Screens/PopupScreen.h"


UIManager::~UIManager()
{
	clearScreens();
}


void UIManager::init(GameController* gameController)
{
	mController.init(gameController, this);
}


void UIManager::preLoad()
{
	std::vector<ScreenType> types{ ScreenType::LoadingScreen };
	mScreenPool.load(types, 1);
}

void UIManager::load()
{
	std::vector<ScreenType> types;
	for (ScreenType type = ScreenType::None + 1; type < ScreenType::Count; type = type + 1)
	{
		types.push_back(type);
	}

	mScreenPool.load(types, 1);
}


Screen* UIManager::screen(ScreenType type)
{
	return mScreenPool.screenRef(type);
}


void UIManager::clearScreens()
{
	// TODO: make sure all screens have been returned first?
	mController.clearScreenStack();
	mScreenPool.freeAll();
}


void UIManager::handleInput(const InputManager* input)
{
#if UI_EDITOR
	mEditor.handleInput(input);
#endif

	mController.getActiveScreen()->handleInput(input);
	mController.getActiveScreen()->updateInputs(input);
}


void UIManager::update() 
{
	mController.processScreenChanges();
	mController.getActiveScreen()->slowUpdate();

#if UI_EDITOR
	mEditor.setScreen(mController.getActiveScreen());
#endif
}


Screen* UIManager::getActiveScreen()
{
	return mController.getActiveScreen();
}


void UIManager::render()
{
	if (mController.persistingScreen())
		mController.persistingScreen()->render();

	mController.getActiveScreen()->render();

#if UI_EDITOR
	mEditor.render();
#endif
		
	mCursor->render();
}


void UIManager::handleEvent(EventData& data)
{
	UIEventHandler eventHandler;
	eventHandler.handleEvent(mController.getActiveScreen(), data);
}


void UIManager::initCursor(Cursor* cursor)
{
	mCursor = cursor;
}


void UIManager::setCursorTexture(Texture* texture) 
{ 
	if (TextureManager::Get()->getTextureName(texture) == "GameCursor")
		mCursor->setCursorOffsetPoint(VectorF());
	else
		mCursor->setCursorOffsetPoint(mCursor->size() / -2.0f);

	mCursor->setTexture(texture);
}
