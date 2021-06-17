#pragma once

class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(const BasicString& filePath);

    // render rect
	void render(const RectF& rect) const;
	void render(const RectF& rect, SDL_RendererFlip flip) const;
	void render(const RectF& rect, double rotation, VectorF aboutPoint) const;
	void render(const RectF& rect, double rotation, VectorF aboutPoint, SDL_RendererFlip flip) const;

    // render sub texture
	void renderSubTexture(const RectF& rect, const RectF& subRect) const;
	void renderSubTexture(const RectF& rect, const RectF& subRect, double rotation, VectorF aboutPoint) const;
	void renderSubTexture(const RectF& rect, const RectF& subRect, double rotation, VectorF aboutPoint, SDL_RendererFlip flip) const;
	void renderSubTexture(const RectF& rect, const RectF& subRect, SDL_RendererFlip flip) const;
	void renderSubTexture(const RectF& rect, const RectF& subRect, SDL_RendererFlip flip, Uint8 tempAlpha);
	void renderSubTexture(const RectF& rect, const RectF& subRect, SDL_RendererFlip flip, RenderColour colourMod);

    // render quad
	void render(const QuadF& quad) const;
    
    // render queue
    void addToRenderQueue(const RectF& rect) const;

	const Uint8 alpha() const;

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
