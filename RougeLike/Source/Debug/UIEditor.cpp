#include "pch.h"
#include "UIEditor.h"

#if UI_EDITOR
#include "UI/Screens/Screen.h"
#include "Input/InputManager.h"

#include "Collisions/Collider.h"
#include "UI/Elements/UIElement.h"

#include "UI/UIEventHandler.h"


void UIEditor::setScreen(Screen* activeScreen)
{
	mScreen = activeScreen;
}


void UIEditor::handleInput(const InputManager* input)
{
	VectorF movement;

	if (input->isCursorPressed(Cursor::Right))
	{
		mElementID = selectElement(input);
	}

	if (!mElementID.empty())
	{
		movement += moveElement(input, Button::UILeft, VectorF(-1.0, 0));
		movement += moveElement(input, Button::UIRight, VectorF(1.0, 0));
		movement += moveElement(input, Button::UIUp, VectorF(0, -1.0));
		movement += moveElement(input, Button::UIDown, VectorF(0, +1.0));
	}


	if (!movement.isZero())
	{
		handleEvent(input, movement);
	}

	if (input->isPressed(Button::Enter) && !mElementID.empty())
	{
		printPosition();
	}
}


void UIEditor::handleEvent(const InputManager* input, VectorF movement)
{
	EditUIRectEvent event(mElementID, movement);

	if (input->isHeld(Button::Ctrl))
		event.setEventType(Event::ChangeUIElementSize);
	else
		event.setEventType(Event::MoveUIElement);

	UIEventHandler eventHandler;
	eventHandler.handleEvent(mScreen, event);
}


void UIEditor::render()
{
	if (!mElementID.empty())
	{
		UIElement* element = mScreen->find(mElementID);
		if (element)
			debugDrawRectOutline(element->rect(), RenderColour::Red);
	}
}


void UIEditor::printPosition()
{
	UIElement* element = mScreen->find(mElementID);
	RectF rect = element->rect();

	const UIElement* parent = element->parent();

	if (parent)
		rect = rect.Translate(parent->rect().TopLeft() * -1.0f);

	DebugPrint(Log, "Element %s rect\nx = \"%.3f\" y = \"%.3f\" width = \"%.3f\" height = \"%.3f\"\n\n", element->id().c_str(), rect.x1, rect.y1, rect.Width(), rect.Height());
}


VectorF UIEditor::moveElement(const InputManager* input, Button::Key key, VectorF direction)
{
	VectorF movementVector;

	if (input->isPressed(key))
	{
		movementVector += direction * movementSpeed;
	}
	else if (input->isHeld(key))
	{
		if (input->getHeldFrames(key) > holdDelay && input->getHeldFrames(key) % holdSpeed == 0)
		{
			movementVector += direction * movementSpeed;
		}
	}

	return movementVector;
}


BasicString UIEditor::selectElement(const InputManager* input)
{
	BasicString elementID;
	Elements elementList = mScreen->layers().elementList();
	for (UIElement* element : elementList)
	{
		Collider collider(&element->rect());

		if (collider.contains(input->cursorPosition()))
		{
			elementID = element->id();
			// No break so we select the highest element
		}
	}

	return elementID;
}
#endif