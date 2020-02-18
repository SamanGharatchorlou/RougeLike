#include "pch.h"
#include "UITextBox.h"


// UI Text
UIBasicText::UIBasicText(Data& data) :
	mText(data.text, data.font, data.ptSize, data.colour),
	UIElement(data)
{

}

void UIBasicText::render()
{
	mText.render(mRect.TopLeft());
}


// UI Text Box
UITextBox::UITextBox(Data& data) :
	mText(data.text, data.font, data.ptSize, data.colour),
	UIBox(data)
{

}

void UITextBox::render()
{
	mText.render(mRect.TopLeft());
	UIBox::render();
}