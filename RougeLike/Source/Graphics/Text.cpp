#include "pch.h"
#include "Text.h"
#include "Renderer.h"

Text::Text(std::string text, std::string font, int ptSize, SDL_Color colour)
{
	mFont.init(Renderer::Get()->renderer());

	FileManager* fm = FileManager::Get();
	mFont.loadFromFile(fm->getFilePath(FileManager::Font, font), ptSize);

	mFont.setColour(colour);
	mFont.setText(text);
}


void Text::init(std::string fontLabel, int ptSize, SDL_Color colour, std::string text)
{
	mFont.init(Renderer::Get()->renderer());

	FileManager* fm = FileManager::Get();
	mFont.loadFromFile(fm->getFilePath(FileManager::Font, fontLabel), ptSize);

	mFont.setColour(colour);
	mFont.setText(text);
}


void Text::render(VectorF position)
{
	mFont.render(position);
}