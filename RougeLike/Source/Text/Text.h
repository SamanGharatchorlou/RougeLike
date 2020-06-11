#pragma once

#include "Font.h"

class Text
{
public:
	Text() { }
	Text(const std::string& text, const std::string& font, int ptSize, SDL_Color colour);
	void init(const std::string& text, const std::string& font, int ptSize, SDL_Color colour);

	void setText(const std::string& text);
	void setColour(SDL_Color colour);

	void displaySubText(int startIndex, int endIndex);

	int fontSize() const { return mFont.ptSize(); }
	void setFontSize(int fontSize) { mFont.resize(fontSize); }

	VectorF size() const { return VectorF(mFont.width(), mFont.height()); }
	VectorF autoSize(VectorF size);
	void setWidth(float width);

	float stringWidth();

	void setOffset(VectorF offset) { mAlignmentOffset = offset; }

	void render(VectorF position);


private:
	std::string mText;
	Font mFont;

	VectorF mAlignmentOffset;
};