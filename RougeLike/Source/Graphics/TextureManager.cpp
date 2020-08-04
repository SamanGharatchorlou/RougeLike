#include "pch.h"
#include "TextureManager.h"
#include "Game/LoadingManager.h"

#include "Texture.h"


TextureManager::TextureManager()
{
	DebugPrint(Log, "Texture manager created\n");
}


TextureManager::~TextureManager()
{
	std::unordered_map<FileManager::Folder, TextureMap>::iterator iter;
	for (iter = mTextures.begin(); iter != mTextures.end(); iter++)
	{
		TextureMap textureMap = iter->second;
		textureMap.free();
	}

	DebugPrint(Log, "Texture manager destroyed\n");
}


// load all textures here
void TextureManager::init() 
{
	FileManager* fm = FileManager::Get();

	// Folders to be loaded
	std::vector<FileManager::Folder> folders;
	std::vector<BasicString> folderPaths = fm->foldersInFolder(FileManager::Images);
	for (int i = 0; i < folderPaths.size(); i++)
	{
		FileManager::Folder folder = fm->getFolderIndex(folderPaths[i]);
		folders.push_back(folder);
	}

	DebugPrint(Log, "\n--- Loading Textures ---\n");
	int fails = 0;

	for (int i = 0; i < folders.size(); i++)
	{
#if _DEBUG
		BasicString folderPath = fm->generatePath(folders[i]);
		BasicString rootPath = fm->generatePath(FileManager::Root);
		int start = rootPath.length();
		int length = folderPath.length() - start;
		folderPath = folderPath.substr(start, length);
		DebugPrint(Log, "\nLoading all textures within the folder '%s'\n", folderPath.c_str());
#endif
		fails += loadAllTexturesIn(folders[i]);
	}

	DebugPrint(Log, "\n--- Texture Loading Complete: %d Failures ---\n\n", fails);
}


int TextureManager::loadAllTexturesIn(FileManager::Folder folder)
{
	TextureMap textureMap;
	int fails = 0;
#if _DEBUG
	int count = 0;
#endif

	std::vector<BasicString> imagePaths = FileManager::Get()->allFilesInFolder(folder);
	for (const BasicString& path : imagePaths)
	{
		fails += !loadTexture(textureMap, path);
#if _DEBUG
		count++;
#endif
	}

#if _DEBUG
	if (textureMap.size() != count)
		DebugPrint(Warning, "The final number of textures does not match the number of file paths provided.\ncount (%d) != map size (%d).\n", count, textureMap.size());
#endif

	mTextures[folder] = textureMap;
	return fails;
}


bool TextureManager::loadTexture(TextureMap& textureMap, const BasicString& filePath)
{
	int mapSize = textureMap.size();
	bool success = true;
	FileManager* fm = FileManager::Get();
	Texture *texture = new Texture;

	Renderer::Get()->Open();
	if (texture->loadFromFile(filePath))
	{
		BasicString label = fm->getItemName(filePath);
		textureMap.add(label, texture);

		// Add to has loaded files
		LoadingManager::Get()->successfullyLoaded(filePath);
		DebugPrint(Log, "Success: loaded texture '%s'\n", label.c_str());
	}
	else
	{
		BasicString label = fm->getItemName(filePath);
		DebugPrint(Log, "Failure: texture NOT loaded '%s' at '%s'\n", label.c_str(), filePath.c_str());
		delete texture;
		success = false;
	}

	Renderer::Get()->Close();
	return success;
}



// TODO: replace with copy
BasicString TextureManager::getTextureName(const Texture* texture) const
{
	std::unordered_map<FileManager::Folder, TextureMap>::const_iterator iter;
	for (iter = mTextures.begin(); iter != mTextures.end(); iter++)
	{
		TextureMap textureMap = iter->second;
		BasicString id = textureMap.find(texture);

		if (!id.empty())
			return id;
	}

	DebugPrint(Log, "Texture was not found within any texture map\n");
	return BasicString();
}


Texture* TextureManager::getTexture(const BasicString& label, const FileManager::Folder folder) const
{
	TextureMap textureMap = findTextureMap(folder);
	Texture* texture = textureMap.find(label);

#if _DEBUG
	if (!texture)
		DebugPrint(Warning, "No item in folder map '%d' with label: '%s'\n", folder, label.c_str());
#endif

	return texture;
}


/// --- Priavte Functions --- ///

const TextureMap& TextureManager::findTextureMap(const FileManager::Folder folder) const
{
	std::unordered_map<FileManager::Folder, TextureMap>::const_iterator iter;
	for (iter = mTextures.begin(); iter != mTextures.end(); iter++)
	{
		if (folder == iter->first)
			return iter->second;
	}

	ASSERT(Error, false, "There is no texture Map in the folder '%s'\n", FileManager::Get()->generatePath(folder).c_str());
}