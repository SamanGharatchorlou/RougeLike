#include "pch.h"
#include "Tileset.h"
#include "Texture.h"


// --- Tile --- //
void Tile::render(Rect<int> targetRect) const
{
	texture->renderSubTexture(targetRect, rect);
}

void Tile::render(Rect<float> targetRect, SDL_RendererFlip flip) const
{
	texture->renderSubTexture(targetRect, rect, flip);
}

void Tile::render(Rect<float> targetRect, SDL_RendererFlip flip, Uint8 alpha)
{
	texture->setAlpha(alpha);
	texture->renderSubTexture(targetRect, rect, flip);
}

Tileset::Tileset(TilesetData& data) :
	mTexture(data.texture),
	tileDimentions(data.tileSize),
	tileCount(data.tileCount)
{
	divideIntoTiles();
}



void Tileset::divideIntoTiles()
{
#if _DEBUG // Tileset Info needs to be setup before dividing into tiles
	validate();
#endif

	mTiles.reserve(tileCount.y);

	for(int y = 0; y < tileCount.y; y++)
	{
		std::vector<Tile> tileRow;
		tileRow.reserve(tileCount.x);

		for(int x = 0; x < tileCount.x ; x++)
		{
			Tile tile;

			tile.setTexture(mTexture);

			Vector2D<int> position = tileDimentions * Vector2D<int>(x, y);
			tile.setTileRect(Rect<int>(position, tileDimentions));

			tileRow.push_back(tile);
		}

		mTiles.push_back(tileRow);
	}
}


Tile& Tileset::getTile(int index)
{
	int x = index % tileCount.x;
	//int y = std::floor(index / tileCount.x);
	int y = index / tileCount.x; // have I broken it?

	ASSERT(Warning, x <= tileCount.x && y < tileCount.y,
		"Animation index (%d, %d) out of sprite sheet bounds (%d, %d)\n",
		x, y, tileCount.x, tileCount.y);

	return mTiles[y][x];
}


Tile& Tileset::getTile(Vector2D<int> index)
{
	ASSERT(Warning, index.x <= tileCount.x && index.y < tileCount.y,
		"Animation index (%d, %d) out of sprite sheet bounds (%d, %d)\n",
		index.x, index.y, tileCount.x, tileCount.y);

	return mTiles[index.y][index.x];
}


Tile& Tileset::getTile(int x, int y)
{
	ASSERT(Warning, x <= tileCount.x && y < tileCount.y,
		"Animation index (%d, %d) out of tileset bounds (%d, %d)\n",
		x, y, tileCount.x, tileCount.y);

	return mTiles[y][x];
}


#if _DEBUG
void Tileset::validate()
{
	if (tileDimentions.isZero() || tileCount.isZero())
	{
		DebugPrint(Warning,
			"The tileset has not been correctly setup, one or more values have not been set\n");
	}
}
#endif 