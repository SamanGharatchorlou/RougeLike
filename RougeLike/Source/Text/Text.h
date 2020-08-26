#pragma once

#include "Font.h"

class Text
{
public:
	Text() { }
	Text(const BasicString& text, const BasicString& font, int ptSize, SDL_Color colour);
	void init(const BasicString& text, const BasicString& font, int ptSize, SDL_Color colour);

	void setText(const BasicString& text);
	void setColour(SDL_Color colour);

	int fontSize() const { return mFont.ptSize(); }
	void setFontSize(int fontSize) { mFont.resize(fontSize); }

	void autoSize(VectorF size);
	void autoSizeWrap(VectorF size);

	VectorF size() const { return mFont.size().toFloat(); }
	void setWidth(float width);

	void setOffset(VectorF offset) { mAlignmentOffset = offset; }

	void render(VectorF position);


private:
	BasicString mText;
	Font mFont;

	VectorF mAlignmentOffset;
};