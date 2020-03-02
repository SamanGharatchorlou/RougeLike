#include "pch.h"
#include "Text.h"
#include "Renderer.h"


Text::Text(const std::string& text, std::string font, int ptSize, SDL_Color colour)
{
	mFont.init(Renderer::Get()->renderer());

	FileManager* fm = FileManager::Get();
	mFont.loadFromFile(fm->getFilePath(FileManager::Font, font), ptSize);

	mFont.setColour(colour);
	setText(text);
}


void Text::init(const std::string& fontLabel, int ptSize, SDL_Color colour, const std::string& text)
{
	mFont.init(Renderer::Get()->renderer());

	FileManager* fm = FileManager::Get();
	mFont.loadFromFile(fm->getFilePath(FileManager::Font, fontLabel), ptSize);

	mFont.setColour(colour);
	setText(text);
}


void Text::setText(const std::string& text) 
{ 
	mText = std::string(text);
	mFont.setText(text); 
}


void Text::render(VectorF position)
{
	mFont.render(position);
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
	while (width < targetWidth || height < targetHeight)
	{
		mFont.resize(++ptSize);
		TTF_SizeText(mFont.get(), mText.c_str(), &width, &height);
	}

	mFont.setText(mText);

	return VectorF(width, height);
}