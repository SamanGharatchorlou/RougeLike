#include "pch.h"
#include "UIManager.h"

#include "UI/Screens/GameScreen.h"
#include "UI/Screens/PauseScreen.h"

#include "ScreenDecoder.h"
#include "Game/GameData.h"

#include "Characters/Attributes/Damage.h"


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
	activeScreen->exit();
	activeScreen = nullptr;

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
	ScreenDecoder screenDecoder(mGameData);

	fs::path menuDirectoryPath = fs::current_path();
	menuDirectoryPath /= "Resources/Configs/UIMenus";
	ASSERT(Warning, fs::is_directory(menuDirectoryPath), "Path: %s is not a directory\n", menuDirectoryPath.string().c_str());

	fs::path menuPath;
	ScreenAttributes attributes;

	//// Pause Menu
	//menuPath = menuDirectoryPath / "PauseScreen.xml";
	//ASSERT(Warning, menuPath.has_filename(), "File: %s does not exist\n", menuPath.string().c_str());

	//attributes = screenDecoder.getScreenAttributes(menuPath.string());
	//std::vector<UILayer*> pauseLayers = screenDecoder.buildUIScreenLayers(attributes);

	//screens.push_back(new PauseScreen(mGameData, pauseLayers));

	// Game Menu
	menuPath = menuDirectoryPath / "GameScreen.xml";	
	ASSERT(Warning, menuPath.has_filename(), "File: %s does not exist\n", menuPath.string().c_str());

	attributes = screenDecoder.getScreenAttributes(menuPath.string());
	std::vector<UILayer*> gameLayers = screenDecoder.buildUIScreenLayers(attributes);
	
	screens.push_back(new GameScreen(mGameData, gameLayers));

	// Default screen selection to... anything
	activeScreen = screens[0];
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
	case Event::UpdateScore:
	{
		UpdateScoreEvent scoreEvent = static_cast<UpdateScoreEvent&>(data);

		std::string scoreString = "Score: " + std::to_string(scoreEvent.mScore);

		UIElement* element = find("Score");

		if (element != nullptr && element->type() == UIElement::BasicText)
		{
			UIBasicText* text = static_cast<UIBasicText*>(element);

			text->setText(scoreString);
		}

		break;
	}
	case Event::SetHealth:
	{
		SetHealthBarEvent event = static_cast<SetHealthBarEvent&>(data);

		UIElement* element = find("RedHealth");

		if (element != nullptr && element->type() == UIElement::Box)
		{
			UIBox* healthBar = static_cast<UIBox*>(element);

			RectF rect = healthBar->getRect();
			rect.SetWidth(rect.Width() * event.health.getPercentage());

			healthBar->setRect(rect);
		}

		break;
	}
	default:
		break;
	}
}