#pragma once

class Texture;
class Font;

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	void init(SDL_Renderer* setRenderer);

	bool loadTexture(std::string label , std::string fileName);

	Texture* getTexture(std::string label);

private:
	SDL_Renderer* renderer;
	std::unordered_map<std::string, Texture*> textures;

	int loadTextureConfigs(std::string textureFolder, std::string config);
};