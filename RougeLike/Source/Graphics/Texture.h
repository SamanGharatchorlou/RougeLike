#pragma once

constexpr Uint8 alphaMax = 255;
constexpr Uint8 alphaMin = 0;


class Texture
{
public:

	enum Flip
	{
		none,
		horizontal,
		vertical
	};


public:
	Texture(SDL_Renderer* renderer);
	~Texture();

	bool loadFromFile(const std::string filePath);

	void render(const Rect<int> rect) const;
	void render(const RectF rect) const;
	void render(const Rect<int> rect, SDL_RendererFlip flip) const;
	void render(const RectF rect, double rotation, VectorF aboutPoint) const;
	void renderSubTexture(const Rect<int> rect, const Rect<int> subRect) const;
	void renderSubTexture(const RectF rect, const Rect<int> subRect) const; 
	void renderSubTexture(const RectF rect, const Rect<int> subRect, double rotation, VectorF aboutPoint) const;
	void renderSubTexture(const RectF rect, const Rect<int> subRect, SDL_RendererFlip flip) const;
	void renderSubTexture(const RectF rect, const Rect<int> subRect, double rotation, VectorF aboutPoint, SDL_RendererFlip flip) const;

	const Uint8 alpha() const;
	void setAlpha(const Uint8 alpha);
	void modifyAlpha(const int delta);

public:
	VectorF originalDimentions; // can I remove this??? -> change it to dimentions

private:
	void free() const;


private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};