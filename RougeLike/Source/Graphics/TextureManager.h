#pragma once

class Texture;

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	void init();

	int loadAllTextures(FileManager::Folder folder);
	bool loadTexture(const std::string& label , const std::string& fileName);

	Texture* getTexture(const std::string& label) const;

private:
	std::unordered_map<std::string, Texture*> textures;
};