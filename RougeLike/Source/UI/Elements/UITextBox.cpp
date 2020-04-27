#include "pch.h"
#include "UITextBox.h"


// UI Text Box
UITextBox::UITextBox(Data& data) :
	mText(data.text, data.font, data.ptSize, data.colour),
	mAlignment(Alignment::None),
	UIBox(data)
{
	if (data.aligment == "Center")
		mAlignment = Alignment::Center;
	if (data.aligment == "Left")
		mAlignment = Alignment::Left;
	else if (data.aligment == "Right")
		mAlignment = Alignment::Right;
}

void UITextBox::render()
{
	mText.render(mRect.TopLeft());
	UIBox::render();
}


void UITextBox::autoSizeFont()
{
	VectorF newSize = mText.autoSize(mRect.Size());
	//mRect.SetSize(newSize);
}

void UITextBox::align()
{
	if (mAlignment == Alignment::Center)
	{
		float textWidth = mText.width();
		float rectWidth = mRect.Width();

		float xOffset = (rectWidth - textWidth) / 2;
		mText.setOffset(VectorF(xOffset, 0.0f));
	}
}