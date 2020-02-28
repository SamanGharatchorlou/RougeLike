#include "pch.h"
#include "UITextBox.h"


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