#include "pch.h"
#include "UIManager.h"

#include "UI/Screens/GameScreen.h"
#include "UI/Screens/PauseScreen.h"

#include "ScreenDecoder.h"
#include "Game/GameData.h"

UIManager::UIManager(GameData* gameData) : mGameData(gameData) { }

UIManager::~UIManager()
{
	for (Screen* screen : screens)
	{
		delete screen;
	}

	screens.clear();
}


void UIManager::selectScreen(Screen::Type screenType)
{
	if (activeScreen)
	{
		activeScreen->exit();
		activeScreen = nullptr;
	}

	for (Screen* screen : screens)
	{
		if (screen->type() == screenType)
		{
			activeScreen = screen;
			activeScreen->enter();
			break;
		}
	}

	ASSERT(Error, activeScreen != nullptr, "No screen was selected, invalid screenType %d\n", screenType);
}


void UIManager::init()
{
	ScreenDecoder screenDecoder(mGameData->textureManager);

	ScreenAttributes attributes;
	std::vector<UILayer*> screenLayers;

	std::string screenPath = FileManager::Get()->folderPath(FileManager::Config_UI);
	std::vector<std::string> screenNames = FileManager::Get()->fileNamesInFolder(FileManager::Config_UI);

	for (const std::string& screenName : screenNames)
	{
		attributes = screenDecoder.getScreenAttributes(screenPath + screenName + ".xml");
		screenLayers = screenDecoder.buildUIScreenLayers(attributes);

		Screen* screen = nullptr;

		if (screenName == "GameScreen")
			screen = new GameScreen(mGameData, screenLayers);

		else if (screenName == "PauseScreen")
			screen = new PauseScreen(mGameData, screenLayers);

		screens.push_back(screen);
	}

	activeScreen = nullptr;
}


UIElement* UIManager::find(std::string id)
{
	for (Screen* screen : screens)
	{
		for (UILayer* layer : screen->layers())
		{
			for (UIElement* element : layer->elements())
			{
				if (element->id() == id)
					return element;
			}
		}
	}

	DebugPrint(Warning, "No element with the id %s was found\n", id.c_str());
	return nullptr;
}



void UIManager::handleEvent(Event event, EventData& data)
{
	switch (event)
	{
	case Event::UpdateTextBox:
	{
		UpdateTextBoxEvent eventData = static_cast<UpdateTextBoxEvent&>(data);

		UIElement* element = find(eventData.mId);

		if (element != nullptr && element->type() == UIElement::Type::TextBox)
		{
			UITextBox* text = static_cast<UITextBox*>(element);
			std::string score = std::to_string(eventData.mValue);

			text->setText(score);
		}

		break;
	}
	case Event::SetHealth:
	{
		SetHealthBarEvent event = static_cast<SetHealthBarEvent&>(data);

		UIElement* element = find("RedHealth");

		if (element != nullptr && element->type() == UIElement::Type::Box)
		{
			UIBox* healthBar = static_cast<UIBox*>(element);

			RectF rect = healthBar->rect();
			rect.SetWidth(rect.Width() * event.health.getPercentage());

			healthBar->setRect(rect);
		}

		break;
	}
	default:
		break;
	}
}