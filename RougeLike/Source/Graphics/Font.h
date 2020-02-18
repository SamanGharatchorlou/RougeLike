#pragma once

class Font
{
public:
	Font() : mRenderer(nullptr), mTexture(nullptr), mFont(nullptr) { }
	~Font();

	void init(SDL_Renderer* renderer) { mRenderer = renderer; }
	bool loadFromFile(std::string font, int ptSize);

	void render(const VectorF position) const;

	//Creates image from font string
	void setColour(SDL_Color textColor) { colour = textColor; }
	void setText(std::string textureText);

private:
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	TTF_Font* mFont;

	SDL_Color colour;

	VectorF size;

	// Forbid copy to prevent shared ownership of of gFont and
	// it being destroyed by the destructor while still in use
	Font(Font& font);
	Font& operator = (const Font) { DebugPrint(Error, "Do no use!\n"); };
};
 