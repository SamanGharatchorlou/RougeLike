#include "pch.h"
#include "UITextBox.h"


UITextBox::UITextBox(const Attributes& attributes) : UIBox(attributes)
{
	initText(attributes);

	BasicString aligment = attributes.contains("align") ? attributes.getString("align") : "";
	setAlignment(aligment);
}

UITextBox::UITextBox(const UITextBoxData data)
{
	Attributes attributes;
	attributes.add("x", data.rect.TopLeft().x);
	attributes.add("y", data.rect.TopLeft().y);
	attributes.add("width", data.rect.Width());
	attributes.add("height", data.rect.Height());
	UIElement::fill(attributes);


	mText.init(data.text, data.font, data.ptSize, data.colour);
	setAlignment(data.alignment);
}


void UITextBox::render()
{
	mText.render(mRect.TopLeft());
	UIBox::render();
}


void UITextBox::autoSizeFont()
{
	VectorF newSize = mText.autoSize(mRect.Size());
}


void UITextBox::align()
{
	if (mAlignment == Alignment::Center)
	{
		float textWidth = mText.size().x;
		float rectWidth = mRect.Width();

		float xOffset = (rectWidth - textWidth) / 2;
		mText.setOffset(VectorF(xOffset, 0.0f));
	}
	else if (mAlignment == Alignment::Left || mAlignment == Alignment::Right)
	{
		DebugPrint(Log, "Unimplemented\n");
	}
}


void UITextBox::autoSizeRectToText()
{
	mRect.SetSize(mText.size());
	align();
}




// -- Private Functions -- //

void UITextBox::setAlignment(const BasicString& alignment)
{
	if (alignment == "Center")
		mAlignment = Alignment::Center;
	else if (alignment == "Left")
		mAlignment = Alignment::Left;
	else if (alignment == "Right")
		mAlignment = Alignment::Right;
	else
		mAlignment = Alignment::None;

	align();
}


void UITextBox::initText(const Attributes& attributes)
{
	BasicString text = attributes.getString("text");
	BasicString font = attributes.contains("font") ? attributes.getString("font") : "default";
	int ptSize = attributes.contains("ptSize") ? attributes.getInt("ptSize") : 0;
	SDL_Color colour = {
		(Uint8)attributes.getInt("r"),
		(Uint8)attributes.getInt("g"),
		(Uint8)attributes.getInt("b")
	};

	mText.init(text, font, ptSize, colour);
}
