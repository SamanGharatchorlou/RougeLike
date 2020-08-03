#pragma once

class Renderer;

class Font
{
public:
	Font() : mTexture(nullptr), mFont(nullptr) { }
	~Font();

	bool loadFromFile(const BasicString& filePath, int ptSize);

	void render(const VectorF position) const;

	//Creates image from font string
	void setColour(SDL_Color textColor) { colour = textColor; }
	void setText(const BasicString& textureText);

	void resize(int ptSize);

	int ptSize() const { return mPtSize; }
	float width() const { return size.x; }
	float height() const { return size.y; }

	TTF_Font* get() { return mFont; }

	const BasicString name() const { return mFontName; }

private:
	Renderer* mRenderer;
	SDL_Texture* mTexture;

	BasicString mFontName;
	TTF_Font* mFont;

	SDL_Color colour;

	int mPtSize;

	VectorF size;

	// Forbid copy to prevent shared ownership of of gFont and
	// it being destroyed by the destructor while still in use
	Font(Font& font);
	Font& operator = (const Font) { DebugPrint(Error, "Do not use!\n"); };
};
 