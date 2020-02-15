#pragma once

#include "Environment/Attributes.h"
#include "Graphics/Tileset.h"

// TODO: this whole class is a bit crap

class LevelMapping
{
public:

	class Layer
	{
	public:
		Layer(Attributes attributes, std::string encoding);

		const std::vector<std::vector<int>>& getMapping() { return mEncoding; }

	private:
		void setAttributes(Attributes attributes);
		void loadEncoding(std::string encoding);

#if _DEBUG
		std::string name;
#endif

		int id;
		Vector2D<int> tileCount;
		std::vector<std::vector<int>> mEncoding;
	};



public:
	void add(Layer layer) { layers.push_back(layer); }

	int getLayerCount() { return layers.size(); }
	Layer& getLayer(int index) { return layers[index]; }

	Vector2D<int> getTileDimentions() { return mLevelInfo.tileDimentions; }
	VectorF getMapDimentions();

	// Check info matches encoding data
	void validateLevelData();

	void setAttributes(Attributes attributes);

private:
	TilesetInfo mLevelInfo;

	std::vector<Layer> layers;
};