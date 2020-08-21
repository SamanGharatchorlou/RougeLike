#include "pch.h"
#include "UIManager.h"

#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"

#include "UIEventHandler.h"

#include "ScreenReader.h"
#include "ScreenBuilder.h"
#include "Elements/UIButton.h"
#include "Elements/UITextBox.h"


UIManager::~UIManager()
{
	for (Screen* screen : mScreens)
	{
		delete screen;
	}
}


void UIManager::init()
{
	Screen* nullScreen = new NullScreen;
	mScreens.push_back(nullScreen);
	mActiveScreen = mScreens.front();
}



Screen* UIManager::screen(Screen::Type type)
{
	for (int i = 0; i < mScreens.size(); i++)
	{
		if (mScreens[i]->type() == type)
			return mScreens[i];
	}

	DebugPrint(Log, "No screen found with type %d\n", type);
	return nullptr;
}


void UIManager::selectScreen(Screen::Type screenType)
{
	mActiveScreen->exit();
	
	for (Screen* screen : mScreens)
	{
		if (screen->type() == screenType)
		{
			mActiveScreen = screen;
			mActiveScreen->enter();
#if UI_EDITOR
			mEditor.setScreen(mActiveScreen);
#endif
			break;
		}
	}
}


void UIManager::setupScreens()
{
	init();

	ScreenBuilder builder;
	std::vector<BasicString> configs = FileManager::Get()->allFilesInFolder(FileManager::Config_Menus);
	for (const BasicString& config : configs)
	{
		if (config == "SettingsScreen")
			continue;
		Screen* screen = builder.buildNewScreen(config);
		mScreens.push_back(screen);
	}
}

void UIManager::clearScreens()
{
	for (int i = 0; i < mScreens.size(); i++)
	{
		delete mScreens[i];
	}

	mScreens.clear();
}


void UIManager::handleInput(const InputManager* input)
{
#if UI_EDITOR
	mEditor.handleInput(input);
#endif

	mActiveScreen->handleInput(input);
	mActiveScreen->updateButtons(input);
}


void UIManager::update(float dt) 
{ 
	mActiveScreen->update();
}


void UIManager::render()
{

	mActiveScreen->render();

#if UI_EDITOR
	mEditor.render();
#endif
		
	mCursor->render();
}


//bool UIManager::isUsingUI() const
//{
//	Elements elements = mActiveScreen->layers().elementList();
//
//	for (UIElement* element : elements)
//	{
//		if (element->type() == UIElement::Type::Button)
//		{
//			UIButton* button = static_cast<UIButton*>(element);
//
//			if (button->isPressed())
//				return true;
//		}
//	}
//
//	return false;
//}


void UIManager::handleEvent(EventData& data)
{
	UIEventHandler eventHandler;
	eventHandler.handleEvent(mActiveScreen, data);
}



void UIManager::initCursor(Cursor* cursor)
{
	mCursor = cursor;
}

void UIManager::setCursorTexture(Texture* texture) 
{ 
	mCursor->setTexture(texture);
}
