#pragma once

#include "Font.h"

class Text
{
public:
	Text() { }
	Text(std::string text, std::string font, int ptSize, SDL_Color colour);
	void init(std::string font, int ptSize, SDL_Color colour, std::string text);

	void setText(std::string text) { mFont.setText(text); }
	void setColour(SDL_Color colour) { mFont.setColour(colour); }

	void render(VectorF position);

private:
	Font mFont;
};