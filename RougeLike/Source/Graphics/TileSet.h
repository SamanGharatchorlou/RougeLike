#pragma once

class Texture;


class Tile
{
public:
	Tile() : texture(nullptr) { }
	virtual ~Tile() {}

	void render(RectF	targetRect) const;
	void render(RectF targetRect, SDL_RendererFlip flip) const;
	void render(RectF targetRect, SDL_RendererFlip flip, Uint8 alpha);

	void setTexture(Texture* tex) { texture = tex; }
	Texture* getTexture() const { return texture; }

	RectF getRect() const { return rect; }
	void setTileRect(RectF tileRect) { rect = tileRect; }

private:
	Texture* texture;
	RectF rect;
};



struct TilesetData
{
	Texture* texture;
	VectorF tileSize;
	Vector2D<int> tileCount;
};


// Dont think i use this anymore?
class Tileset
{
public:
	Tileset() : mTexture(nullptr) { }
	Tileset(TilesetData& data);

	Tile& getTile(Index index);


private:
	Texture* mTexture;
	VectorF tileDimentions;

	Grid<Tile> mTiles;
};


