#pragma once

#if _DEBUG
#include "MapTile.h"

void renderSurfaceTypes(const Grid<MapTile>& data);
void renderCollisionTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize);
void renderRenderTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize);
void renderDecorTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize);
#endif