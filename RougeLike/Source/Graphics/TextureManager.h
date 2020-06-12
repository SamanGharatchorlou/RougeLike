#pragma once

class Texture;

using TextureMap = std::unordered_map<std::string, Texture*>;

using FolderTextureMap = std::pair<FileManager::Folder, TextureMap>;

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	void init();

	Texture* getTexture(const std::string& label, const FileManager::Folder folders) const;
	Texture* getTexture(const std::string& label) const;

	const std::string& getTextureName(const Texture* texture) const;

private:
	bool loadTexture(FolderTextureMap& folderMap, const std::string& fileName);
	int loadAllTexturesIn(FileManager::Folder folder);

	const TextureMap& findTextureMap(const FileManager::Folder folder) const;


private:

	std::vector<FolderTextureMap> mTextures;
};