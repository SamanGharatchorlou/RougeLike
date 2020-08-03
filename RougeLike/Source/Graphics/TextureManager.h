#pragma once

class Texture;

using TextureMap = std::unordered_map<BasicString, Texture*>;
using FolderTextureMap = std::pair<FileManager::Folder, TextureMap>;

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	void init();

	Texture* getTexture(const BasicString& label, const FileManager::Folder folders) const;
	Texture* getTexture(const BasicString& label) const;

	const BasicString& getTextureName(const Texture* texture) const;

private:
	bool loadTexture(FolderTextureMap& folderMap, const BasicString& fileName);
	int loadAllTexturesIn(FileManager::Folder folder);

	const TextureMap& findTextureMap(const FileManager::Folder folder) const;


private:

	std::vector<FolderTextureMap> mTextures;
};