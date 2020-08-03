#include "pch.h"
#include "Text.h"


Text::Text(const BasicString& text, const BasicString& font, int ptSize, SDL_Color colour)
{
	BasicString path = FileManager::Get()->findFile(FileManager::Font, font);
	mFont.loadFromFile(path, ptSize);

	mFont.setColour(colour);
	setText(text);
}


void Text::init(const BasicString& text, const BasicString& font, int ptSize, SDL_Color colour)
{
	BasicString path = FileManager::Get()->findFile(FileManager::Font, font);
	mFont.loadFromFile(path, ptSize);

	mFont.setColour(colour);
	setText(text);
}

void Text::setColour(SDL_Color colour)
{
	mFont.setColour(colour);
	mFont.setText(mText);
}


void Text::setText(const BasicString& text) 
{ 
	mText = text;
	mFont.setText(mText); 
}


//void Text::displaySubText(int startIndex, int endIndex)
//{
//	mFont.setText(mText.substr(startIndex, endIndex - startIndex));
//}


void Text::render(VectorF position)
{
	mFont.render(position + mAlignmentOffset);
}


void Text::setWidth(float width)
{
	int textWidth = -1;
	int desiredWidth = (int)width;
	int ptSize = mFont.ptSize();

	TTF_SizeText(mFont.get(), mText.c_str(), &textWidth, nullptr);

	// increase font size
	if (textWidth < desiredWidth)
	{
		while (textWidth < desiredWidth)
		{
			mFont.resize(++ptSize);
			TTF_SizeText(mFont.get(), mText.c_str(), &textWidth, nullptr);
		}
	}
	// decrease font size
	else
	{
		while (textWidth > desiredWidth)
		{
			mFont.resize(--ptSize);
			TTF_SizeText(mFont.get(), mText.c_str(), &textWidth, nullptr);
		}
	}

	mFont.setText(mText);
}


VectorF Text::autoSize(VectorF size)
{
	int width = -1;
	int height = -1;

	int targetWidth = (int)size.x;
	int targetHeight = (int)size.y;

	int ptSize = mFont.ptSize();

	TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);

	// assume ptSize = 0; so its always smaller
	while (width < targetWidth && height < targetHeight)
	{
		mFont.resize(++ptSize);
		TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);
	}

	mFont.setText(mText);

	return VectorF(width, height);
}

float Text::stringWidth()
{
	float width = -1.0f;
	float height = -1.0f;

	return 0.0f;
}