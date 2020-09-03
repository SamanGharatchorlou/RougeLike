#pragma once

#if DEBUG_CHECK
#include "Map/Tiles/MapTile.h"

class Map;

void renderMapBoundaries(const Map* map);
void renderSurfaceTypes(const Grid<MapTile>& data);
void renderTileIndexes(const Grid<MapTile>& data, Index index, int fontSize);
void renderCollisionTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize);
void renderRenderTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize);
void renderDecorTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize);
#endif