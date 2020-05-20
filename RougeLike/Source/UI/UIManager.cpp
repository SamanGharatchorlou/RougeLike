#include "pch.h"
#include "UIManager.h"

#include "Game/GameData.h"
#include "ScreenDecoder.h"
#include "Screens/ScreenHeaders.h"

#include "UI/Screens/UILayer.h"

// TEMP
#include "Input/InputManager.h"
#include "Collisions/Collider.h"



UIManager::UIManager(GameData* gameData) : mGameData(gameData)
{
#if UI_EDITOR
	elementId = "";
#endif
}


UIManager::~UIManager()
{
	for (Screen* screen : screens)
	{
		delete screen;
	}

	screens.clear();
}


void UIManager::clearScreens()
{
	activeScreen = nullptr;

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

	// Input is only registed for the active screen so refresh this.
	handleInput();
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
			screen = new GameScreen(mGameData);

		else if (screenName == "PauseScreen")
			screen = new PauseScreen(mGameData);

		else if (screenName == "CharacterSelectionScreen")
			screen = new CharacterSelectionScreen(mGameData);

		else if (screenName == "SettingsScreen")
			screen = new SettingsScreen(mGameData);

		if (screen)
		{
			screen->set(screenLayers);
			screens.push_back(screen);
		}
		else
			DebugPrint(Log, "No screen was built from '%s'\n", screenName.c_str());
	}

	activeScreen = nullptr;
}

void UIManager::handleInput()
{
	InputManager* input = mGameData->inputManager;

#if UI_EDITOR
	debugEditUI();
#endif

	for (UILayer* layer : activeScreen->layers())
	{
		for (UIElement* element : layer->elements())
		{
			if (element->type() == UIElement::Type::Button)
			{
				UIButton* button = static_cast<UIButton*>(element);

				if (button->isPointInBounds(input->cursorPosition()))
				{
					button->setState(UIButton::State::Hovering);

					button->setPressed(input->isCursorPressed(Cursor::Left));
					button->setHeld(input->isCursorHeld(Cursor::Left));
					button->setReleased(input->isCursorReleased(Cursor::Left));
				}
				else
				{
					button->reset();
				}
			}
		}
	}
}


void UIManager::update(float dt) 
{ 
	if (activeScreen)
		activeScreen->update(dt);
}


void UIManager::render()
{
	if (activeScreen)
	{
		activeScreen->render();

#if UI_EDITOR
		if (!elementId.empty())
		{
			UIElement* element = findElement(elementId);
			if (element)
				debugDrawRectOutline(element->rect(), RenderColour::Red);
		}
#endif
		
		mGameData->inputManager->getCursor()->render();
	}
}


void UIManager::refresh(Screen::Type screenType)
{
	for (Screen* screen : screens)
	{
		if (screen->type() == screenType)
		{
			ScreenDecoder screenDecoder(mGameData->textureManager);

			std::string screenPath = FileManager::Get()->folderPath(FileManager::Config_UI);
			std::string screenName = typeToString(screenType);

			ScreenAttributes attributes = screenDecoder.getScreenAttributes(screenPath + screenName + ".xml");
			std::vector<UILayer*> screenLayers = screenDecoder.buildUIScreenLayers(attributes);

			screen->set(screenLayers);
		}
	}
}


std::string UIManager::typeToString(Screen::Type screenType)
{
	if (screenType == Screen::Game)
	{
		return "GameScreen";
	}
	else if (screenType == Screen::Pause)
	{
		return "PasueScreen";
	}
	else if (screenType == Screen::CharacterSelection)
	{
		return "CharacterSelectionScreen";
	}
	else
	{
		DebugPrint(Warning, "No screen is associated with the screen type enum %d\n", screenType);
		return "";
	}
}


UIElement* UIManager::findElement(const std::string& id)
{
	for (UILayer* layer : activeScreen->layers())
	{
		for (UIElement* element : layer->elements())
		{
			if (element->id() == id)
				return element;
		}
	}

	DebugPrint(Warning, "No element with the id %s was found on the current active screen\n", id.c_str());
	return nullptr;
}


UIButton* UIManager::findButton(const std::string& id)
{
	for (UILayer* layer : activeScreen->layers())
	{
		for (UIElement* element : layer->elements())
		{
			if (element->id() == id)
			{
				if (element->type() == UIElement::Type::Button)
					return static_cast<UIButton*>(element);
				else
					DebugPrint(Log, "Attemping to use UIManager::findButton on a none button element, ID: '%s'\n", id.c_str());
			}
		}
	}

	return nullptr;
}


bool UIManager::isUsingUI() const
{
	for (UILayer* layer : activeScreen->layers())
	{
		for (UIElement* element : layer->elements())
		{
			if (element->type() == UIElement::Type::Button)
			{
				UIButton* button = static_cast<UIButton*>(element);

				if (button->isPressed())
					return true;
			}
		}
	}

	return false;
}

void UIManager::updateTextBox(UpdateTextBoxEvent& eventData)
{
	UIElement* element = findElement(eventData.mId);

	if (element != nullptr && element->type() == UIElement::Type::TextBox)
	{
		UITextBox* text = static_cast<UITextBox*>(element);
		std::string score = std::to_string(eventData.mValue);

		text->setText(score);
	}
}


void UIManager::handleEvent(EventData& data)
{
	switch (data.eventType)
	{
	case Event::UpdateTextBox:
	{
		updateTextBox(static_cast<UpdateTextBoxEvent&>(data));
		break;
	}
	case Event::SetHealth:
	{
		setHealth(static_cast<SetHealthBarEvent&>(data));
		break;
	}
	case Event::MoveUIElement:
	{
		moveElement(static_cast<EditUIRectEvent&>(data));
		break;
	}

	case Event::ChangeUIElementSize:
	{
		setElementSize(static_cast<EditUIRectEvent&>(data));
		break;
	}
	case Event::SetUIRect:
	{
		setRect(static_cast<SetUIRectEvent&>(data));
		break;
	}
	default:
		break;
	}
}


void UIManager::setCursorTexture(Texture* texture) 
{ 
	mGameData->inputManager->getCursor()->setTexture(texture); 
}



// --- Private Functions --- //


// Event handling functions
void UIManager::setHealth(SetHealthBarEvent& eventData)
{
	UIElement* redHealth = findElement("RedHealth");
	UIElement* blackHealth = findElement("BlackHealth");

	if (redHealth != nullptr && redHealth->type() == UIElement::Type::Box &&
		blackHealth != nullptr && blackHealth->type() == UIElement::Type::Box)
	{
		UIBox* redHealthBar = static_cast<UIBox*>(redHealth);
		UIBox* blackHealthBar = static_cast<UIBox*>(blackHealth);

		RectF hpRect = redHealthBar->rect();
		RectF maxHpRect = blackHealthBar->rect();

		hpRect.SetWidth(maxHpRect.Width() * eventData.health.getPercentage());

		redHealthBar->setRect(hpRect);
	}
}
void UIManager::moveElement(EditUIRectEvent& eventData)
{
	UIElement* element = findElement(eventData.mId);

	if (element != nullptr)
	{
		VectorF movement = eventData.mChange;
		RectF rect = element->rect();
		rect = rect.Translate(movement);
		element->setRect(rect);
	}
}
void UIManager::setElementSize(EditUIRectEvent& eventData)
{
	UIElement* element = findElement(eventData.mId);

	if (element != nullptr)
	{
		VectorF sizeChange = eventData.mChange;
		RectF rect = element->rect();
		rect.SetSize(rect.Size() + sizeChange);

		element->setRect(rect);
	}

}
void UIManager::setRect(SetUIRectEvent& eventData)
{
	UIElement* element = findElement(eventData.mId);

	if (element != nullptr)
	{
		element->setRect(eventData.mRect);
	}
}



#if UI_EDITOR
void UIManager::debugEditUI()
{
	InputManager* input = mGameData->inputManager;

	if (input->isCursorPressed(Cursor::Right))
	{
		for (UILayer* layer : activeScreen->layers())
		{
			for (UIElement* element : layer->elements())
			{
				Collider collider(&element->rect());

				if (collider.contains(input->cursorPosition()))
				{
					elementId = element->id();
					// No break so we select the highest element
				}
			}
		}
	}

	VectorF change;
	float movementSpeed = 1.0f; // pixels moved per press
	int holdSpeed = 1; // larger is slower, using % for it
	int holdDelay = 15;

	// LEFT
	if (input->isPressed(Button::UILeft) && !elementId.empty())
	{
		change += VectorF(-movementSpeed, 0.0f);
	}
	else if (input->isHeld(Button::UILeft) && !elementId.empty())
	{
		if (input->getHeldFrames(Button::UILeft) > holdDelay && input->getHeldFrames(Button::UILeft) % holdSpeed == 0)
		{
			change += VectorF(-movementSpeed, 0.0f);
		}
	}

	// RIGHT
	if (input->isPressed(Button::UIRight) && !elementId.empty())
	{
		change += VectorF(+movementSpeed, 0.0f);
	}
	else if (input->isHeld(Button::UIRight) && !elementId.empty())
	{
		if (input->getHeldFrames(Button::UIRight) > holdDelay && input->getHeldFrames(Button::UIRight) % holdSpeed == 0)
		{
			change += VectorF(+movementSpeed, 0.0f);
		}
	}

	// UP
	if (input->isPressed(Button::UIUp) && !elementId.empty())
	{
		change += VectorF(0.0f, -movementSpeed);
	}
	else if (input->isHeld(Button::UIUp) && !elementId.empty())
	{
		if (input->getHeldFrames(Button::UIUp) > holdDelay && input->getHeldFrames(Button::UIUp) % holdSpeed == 0)
		{
			change += VectorF(0.0f, -movementSpeed);
		}
	}

	// DOWN
	if (input->isPressed(Button::UIDown) && !elementId.empty())
	{
		change += VectorF(0.0f, +movementSpeed);
	}
	else if (input->isHeld(Button::UIDown) && !elementId.empty())
	{
		if (input->getHeldFrames(Button::UIDown) > holdDelay && input->getHeldFrames(Button::UIDown) % holdSpeed == 0)
		{
			change += VectorF(0.0f, +movementSpeed);
		}
	}

	if (!change.isZero())
	{
		EditUIRectEvent event(elementId, change);

		if (input->isHeld(Button::Ctrl))
			event.setEventType(Event::ChangeUIElementSize);
		else
			event.setEventType(Event::MoveUIElement);

		handleEvent(event);
	}

	if (input->isPressed(Button::Enter) && !elementId.empty())
	{
		UIElement* element = findElement(elementId);
		RectF rect = element->rect();

		const UIElement* parent = element->parent();

		if (parent)
			rect = rect.Translate(parent->rect().TopLeft() * -1.0f);

		DebugPrint(Log, "Element %s rect\nx = \"%.3f\" y = \"%.3f\" width = \"%.3f\" height = \"%.3f\"\n\n", element->id().c_str(), rect.x1, rect.y1, rect.Width(), rect.Height());
	}
}
#endif
