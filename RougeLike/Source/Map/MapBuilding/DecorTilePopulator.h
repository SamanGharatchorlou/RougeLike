#pragma once


class MapTile;


class DecorTilePopulator
{
public:
	void fillDecorInfo(Grid<MapTile>& data);

private:
	void addColumns(Grid<MapTile>& data);
	void addWater(Grid<MapTile>& data);
	void addGrating(Grid<MapTile>& data);
};


