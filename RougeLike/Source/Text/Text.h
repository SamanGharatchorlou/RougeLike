#pragma once

#include "Font.h"

class Text
{
public:
	Text() { }
	Text(const std::string& text, std::string font, int ptSize, SDL_Color colour);
	void init(const std::string& font, int ptSize, SDL_Color colour, const std::string& text);

	void setText(const std::string& text);
	void setColour(SDL_Color colour) { mFont.setColour(colour); }

	void displaySubText(int startIndex, int endIndex);

	int fontSize() const { return mFont.ptSize(); }
	void setFontSize(int fontSize) { mFont.resize(fontSize); }

	VectorF autoSize(VectorF size);
	void setWidth(float width);

	float width() const { return mFont.width(); }
	float height() const { return mFont.height(); }

	void render(VectorF position);


private:
	std::string mText;
	Font mFont;
};