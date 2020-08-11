#include "pch.h"
#include "UIEventHandler.h"

#include "Screens/Screen.h"
#include "Elements/UIElement.h"
#include "Elements/UIButton.h"
#include "Elements/UITextBox.h"



void UIEventHandler::handleEvent(Screen* activeScreen, EventData& data)
{
	switch (data.eventType)
	{
	case Event::UpdateTextBox:
	{
		updateTextBox(activeScreen, static_cast<UpdateTextBoxEvent&>(data));
		break;
	}
	case Event::SetUIBar:
	{
		setUIbar(activeScreen, static_cast<SetUIBarEvent&>(data));
		break;
	}
	case Event::SetTextColour:
	{
		setTextColour(activeScreen, static_cast<SetTextColourEvent&>(data));
		break;
	}
#if UI_EDITOR
	case Event::MoveUIElement:
	{
		moveElement(activeScreen, static_cast<EditUIRectEvent&>(data));
		break;
	}
	case Event::ChangeUIElementSize:
	{
		setElementSize(activeScreen, static_cast<EditUIRectEvent&>(data));
		break;
	}
#endif
	default:
		break;
	}
}


void UIEventHandler::updateTextBox(Screen* activeScreen, UpdateTextBoxEvent& eventData)
{
	UIElement* element = activeScreen->find(eventData.mId);

	if (element != nullptr && element->type() == UIElement::Type::TextBox)
	{
		UITextBox* text = static_cast<UITextBox*>(element);
		char character = (char)(eventData.mValue);
		BasicString score(eventData.mValue);

		text->setText(score);
	}
}


void UIEventHandler::setUIbar(Screen* activeScreen, SetUIBarEvent& eventData)
{
	UIElement* bar = activeScreen->find(eventData.mBar);
	UIElement* container = activeScreen->find(eventData.mBarContainer);

	if (bar != nullptr && bar->type() == UIElement::Type::Box &&
		container != nullptr && container->type() == UIElement::Type::Box)
	{
		UIBox* barBox = static_cast<UIBox*>(bar);
		UIBox* containerBox = static_cast<UIBox*>(container);

		RectF barRect = barBox->rect();
		RectF containerRect = containerBox->rect();

		barRect.SetWidth(containerRect.Width() * eventData.mPercentage);

		barBox->setRect(barRect);
	}
}


void UIEventHandler::setTextColour(Screen* activeScreen, SetTextColourEvent& eventData)
{
	UIElement* element = activeScreen->find(eventData.mId);

	if (element != nullptr && element->type() == UIElement::Type::TextBox)
	{
		UITextBox* textBox = static_cast<UITextBox*>(element);
		textBox->setColour(eventData.mColour);
	}
}


#if UI_EDITOR
void UIEventHandler::moveElement(Screen* activeScreen, EditUIRectEvent& eventData)
{
	UIElement* element = activeScreen->find(eventData.mId);

	if (element != nullptr)
	{
		VectorF movement = eventData.mChange;
		RectF rect = element->rect();
		rect = rect.Translate(movement);
		element->setRect(rect);
	}
}

void UIEventHandler::setElementSize(Screen* activeScreen, EditUIRectEvent& eventData)
{
	UIElement* element = activeScreen->find(eventData.mId);

	if (element != nullptr)
	{
		VectorF sizeChange = eventData.mChange;
		RectF rect = element->rect();
		rect.SetSize(rect.Size() + sizeChange);

		element->setRect(rect);
	}

}
#endif