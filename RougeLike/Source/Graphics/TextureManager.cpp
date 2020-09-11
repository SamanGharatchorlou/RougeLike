#include "pch.h"
#include "TextureManager.h"

#include "Renderer.h"
#include "Texture.h"
#include "Game/Data/LoadingManager.h"



TextureManager::TextureManager()
{
	DebugPrint(Log, "Texture manager created\n");
}


TextureManager::~TextureManager()
{
	DebugPrint(Log, "Texture manager destroyed\n");
}


TextureManager* TextureManager::Get()
{
	static TextureManager sInstance;
	return &sInstance;
}


void TextureManager::unload()
{
	std::unordered_map<FileManager::Folder, TextureMap>::iterator iter;
	for (iter = mTextures.begin(); iter != mTextures.end(); iter++)
	{
		TextureMap textureMap = iter->second;
		textureMap.free();
	}

	mTextures.clear();

	DebugPrint(Log, "Texture manager unloaded\n");
}


void TextureManager::preLoad()
{
	loadAllTexturesIn(FileManager::PreLoadFiles);
}


// load all textures here
void TextureManager::load()
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
#if DEBUG_CHECK
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
#if DEBUG_CHECK
	int count = 0;
#endif

	std::vector<BasicString> imagePaths = FileManager::Get()->allFilesInFolder(folder);
	for (const BasicString& path : imagePaths)
	{
		fails += !loadTexture(textureMap, path);
#if DEBUG_CHECK
		count++;
#endif
	}

#if DEBUG_CHECK
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
	Texture* texture = nullptr;
	const TextureMap* textureMap = findTextureMap(folder);

	if(textureMap)
		texture = textureMap->find(label);

	if (!texture)
	{
		if (!label.empty())
		{
			DebugPrint(Log, "No item in folder map '%d' with label: '%s'\n", folder, label.c_str());

			texture = searchAllFiles(label);
			if (!texture)
			{
				DebugPrint(Warning, "No image file with label: '%s' exists in any loaded folder\n", label.c_str());
			}
		}
		// else empty label
	}

	return texture;
}


// --- Priavte Functions --- //

const TextureMap* TextureManager::findTextureMap(const FileManager::Folder folder) const
{
	std::unordered_map<FileManager::Folder, TextureMap>::const_iterator iter;
	for (iter = mTextures.begin(); iter != mTextures.end(); iter++)
	{
		if (folder == iter->first)
			return &iter->second;
	}

	DebugPrint(Warning, "There is no texture Map in the folder '%s'\n", FileManager::Get()->generatePath(folder).c_str());
	return nullptr;
}



Texture* TextureManager::searchAllFiles(const BasicString& label) const
{
	Texture* texture = nullptr;

	std::unordered_map<FileManager::Folder, TextureMap>::const_iterator iter;
	for (iter = mTextures.begin(); iter != mTextures.end(); iter++)
	{
		const TextureMap* textureMap = findTextureMap(iter->first);
		texture = textureMap->find(label);

		if (texture != nullptr)
			break;
	}

	return texture;
}