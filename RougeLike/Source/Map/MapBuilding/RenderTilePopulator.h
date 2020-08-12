#pragma once

class MapTile;

class RenderTilePopulator
{
public:
	void fillRenderInfo(Grid<MapTile>& data);

private:
	void topBottom(Grid<MapTile>& data);
	void leftRight(Grid<MapTile>& data);
	void corners(Grid<MapTile>& data);
	void pointCorners(Grid<MapTile>& data);
	void cleanRenderLabels(Grid<MapTile>& data);
};


