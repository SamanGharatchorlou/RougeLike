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
	int loadAllTextures(FileManager::Folder folder);

	Texture* getTexture(std::string label);

private:
	SDL_Renderer* renderer; // TODO: replace with singleton
	std::unordered_map<std::string, Texture*> textures;
};