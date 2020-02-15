#pragma once

#include "Environment/Attributes.h"
#include "Graphics/Tileset.h"

class MapTileset : public Tileset
{
public:
	MapTileset(Attributes attributes) : mAttributes(attributes) { }

	void init(Texture* tileSetTexture) override;

	std::string getTilesetDataPath();
	void setAttributes(Attributes tilesetAttributes);

private:
	Attributes mAttributes;
};