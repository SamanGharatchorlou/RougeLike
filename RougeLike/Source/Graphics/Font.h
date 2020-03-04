#pragma once

class Font
{
public:
	Font() : mRenderer(nullptr), mTexture(nullptr), mFont(nullptr) { }
	~Font();

	bool loadFromFile(const std::string& font, int ptSize);

	void render(const VectorF position) const;

	//Creates image from font string
	void setColour(SDL_Color textColor) { colour = textColor; }
	void setText(const std::string& textureText);

	void resize(int ptSize);

	int ptSize() const { return mPtSize; }
	float width() const { return size.x; }
	float height() const { return size.y; }

	TTF_Font* get() { return mFont; }

	std::string name() const { return mFontName; }

private:
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;

	std::string mFontName;
	TTF_Font* mFont;

	SDL_Color colour;

	int mPtSize;

	// remove this?
	VectorF size;

	// Forbid copy to prevent shared ownership of of gFont and
	// it being destroyed by the destructor while still in use
	Font(Font& font);
	Font& operator = (const Font) { DebugPrint(Error, "Do no use!\n"); };
};
 