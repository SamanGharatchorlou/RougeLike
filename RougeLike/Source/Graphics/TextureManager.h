#pragma once

class Texture;
class Font;

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	void init();

	bool loadTexture(const std::string& label , const std::string& fileName);
	int loadAllTextures(FileManager::Folder folder);

	Texture* getTexture(const std::string& label) const;

private:
	std::unordered_map<std::string, Texture*> textures;
};