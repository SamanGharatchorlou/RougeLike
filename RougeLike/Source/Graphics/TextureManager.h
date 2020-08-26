#pragma once

#include "TextureMap.h"

class Texture;

class TextureManager
{

public:
	static TextureManager* Get();

	void preLoad();
	void load();
	void unload();

	Texture* getTexture(const BasicString& label, const FileManager::Folder folders) const;


	BasicString getTextureName(const Texture* texture) const;


private:
	bool loadTexture(TextureMap& textureMap, const BasicString& fileName);
	int loadAllTexturesIn(FileManager::Folder folder);

	const TextureMap* findTextureMap(const FileManager::Folder folder) const;


	Texture* searchAllFiles(const BasicString& label) const;


private:
	TextureManager();
	~TextureManager();


private:
	std::unordered_map<FileManager::Folder, TextureMap> mTextures;
	 
};

