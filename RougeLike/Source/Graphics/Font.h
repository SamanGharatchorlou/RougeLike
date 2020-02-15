#pragma once

class Font
{
public:
	Font(SDL_Renderer* renderer);
	~Font();

	bool loadFromFile(const std::string font);

	void render(const Rect<int> rect) const;
	void render(const RectF rect) const;
	void render(const Rect<int> rect, SDL_RendererFlip flip) const;
	void render(const RectF rect, double rotation, VectorF aboutPoint) const;


	//Creates image from font string
	bool setText(std::string textureText, SDL_Color textColor);

	VectorF originalDimentions;


private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	TTF_Font* gFont;
};