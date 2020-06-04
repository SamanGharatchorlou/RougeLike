#pragma once

class Texture;


class Tile
{
public:
	Tile() : texture(nullptr) { }
	virtual ~Tile() {}

	void render(Rect<int>	targetRect) const;
	void render(Rect<float> targetRect, SDL_RendererFlip flip) const;
	void render(Rect<float> targetRect, SDL_RendererFlip flip, Uint8 alpha);

	void setTexture(Texture* tex) { texture = tex; }
	Texture* getTexture() const { return texture; }

	Rect<int> getRect() const { return rect; }
	void setTileRect(Rect<int> tileRect) { rect = tileRect; }

private:
	Texture* texture;

	// specifies which tile within the tileset to render
	Rect<int> rect;
};



struct TilesetData
{
	Texture* texture;
	Vector2D<int> tileSize;
	Vector2D<int> tileCount;
};



class Tileset
{
public:
	Tileset() : mTexture(nullptr) { }
	Tileset(TilesetData& data);

	Tile& getTile(int index);
	Tile& getTile(Vector2D<int> index);
	Tile& getTile(int x, int y);

#if _DEBUG
	void validate();
#endif

private:
	// One sheet, multiple frames - TileSheet
	void divideSheetIntoFrames();

protected:
	Texture* mTexture;

	Vector2D<int> tileDimentions;
	Vector2D<int> tileCount;

private:
	std::vector<std::vector<Tile>> mTiles;
};
