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
	setText(mText);
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


void Text::autoSize(VectorF size)
{
	int width = -1;
	int height = -1;

	int targetWidth = (int)size.x;
	int targetHeight = (int)size.y;

	int ptSize = mFont.ptSize();

	TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);

	if (width > targetWidth || height > targetHeight)
	{
		while (width > targetWidth || height > targetHeight)
		{
			mFont.resize(--ptSize);
			TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);
		}
	}
	else
	{
		while (width < targetWidth && height < targetHeight)
		{
			mFont.resize(++ptSize);
			TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);
		}
	}

	mFont.setText(mText);
}



void Text::autoSizeWrap(VectorF size)
{
	int width = -1;
	int height = -1;

	int targetWidth = (int)size.x;
	int targetHeight = (int)size.y;

	int ptSize = mFont.ptSize();

	TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);

	int numberOfLines = ceilf((float)width / size.x);
	int currentHeight = numberOfLines * height;


	if (currentHeight > targetHeight)
	{
		while (currentHeight > targetHeight)
		{
			numberOfLines = ceilf((float)width / size.x);
			currentHeight = numberOfLines * height;

			mFont.resize(--ptSize);
			TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);
		}
	}
	else
	{
		while (width < targetWidth && height < targetHeight)
		{
			mFont.resize(++ptSize);
			TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);
		}
	}

	mFont.setWrappedText(mText, targetWidth);
}