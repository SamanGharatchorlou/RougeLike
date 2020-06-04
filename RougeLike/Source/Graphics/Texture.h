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
	Texture();
	~Texture();

	bool loadFromFile(const std::string& filePath);

	void render(const Rect<int> rect) const;
	void render(const RectF rect) const;
	void render(const Rect<int> rect, SDL_RendererFlip flip) const;
	void render(const RectF rect, SDL_RendererFlip flip) const;
	void render(const RectF rect, double rotation, VectorF aboutPoint) const;
	void renderSubTexture(const Rect<int> rect, const Rect<int> subRect) const;
	void renderSubTexture(const RectF rect, const Rect<int> subRect) const;
	void renderSubTexture(const RectF rect, const Rect<int> subRect, double rotation, VectorF aboutPoint) const;
	void renderSubTexture(const RectF rect, const Rect<int> subRect, SDL_RendererFlip flip) const;
	void renderSubTexture(const RectF rect, const Rect<int> subRect, SDL_RendererFlip flip, const Uint8 tempAlpha);
	void renderSubTexture(const RectF rect, const Rect<int> subRect, double rotation, VectorF aboutPoint, SDL_RendererFlip flip) const;

	const Uint8 alpha() const;
	void setAlpha(const Uint8 alpha);
	void modifyAlpha(const int delta);


public:
	VectorF originalDimentions;


private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};


class TextureGroup
{
public:
	void add(Texture* texture) { mTextures.push_back(texture); }

private:
	std::vector<Texture*> mTextures;

};