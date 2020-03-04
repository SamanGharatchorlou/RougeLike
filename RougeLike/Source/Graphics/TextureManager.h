#pragma once

class Texture;
class Font;

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	void init(SDL_Renderer* setRenderer);

	bool loadTexture(const std::string label , const std::string& fileName);
	int loadAllTextures(FileManager::Folder folder);

	Texture* getTexture(const std::string& label) const;

private:
	SDL_Renderer* renderer; // TODO: replace with singleton
	std::unordered_map<std::string, Texture*> textures;
};