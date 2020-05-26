#pragma once

class Texture;

using TextureMap = std::unordered_map<std::string, Texture*>;
using FolderMap = std::pair<FileManager::Folder, TextureMap>;

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	void init();

	Texture* getTexture(const std::string& label, const FileManager::Folder folders) const;
	Texture* getTexture(const std::string& label, std::vector<FileManager::Folder> folders) const;
	const std::string& getTextureName(const Texture* texture) const;

private:
	bool loadTexture(FolderMap& folderMap, const std::string& fileName);
	int loadAllTexturesIn(FileManager::Folder folder);

	const TextureMap& findTextureMap(const FileManager::Folder folder) const;


private:

	std::vector<FolderMap> mTextures;
};