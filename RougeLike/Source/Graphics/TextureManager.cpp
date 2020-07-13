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
	//iterate through the map and delete all textures
	for (int i = 0; i < mTextures.size(); i++)
	{
		TextureMap textureMap = mTextures[i].second;

		std::unordered_map<std::string, Texture*>::iterator iter = textureMap.begin();
		for (; iter != textureMap.end(); iter++)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}


	DebugPrint(Log, "Texture manager destroyed\n");
}


// load all textures here
void TextureManager::init() 
{
	FileManager* fm = FileManager::Get();

	// Folders to be loaded
	std::vector<FileManager::Folder> folders;
	std::vector<std::string> folderPaths = fm->foldersInFolder(FileManager::Images);
	for (int i = 0; i < folderPaths.size(); i++)
	{
		FileManager::Folder folder = fm->getFolderIndex(folderPaths[i]);
		folders.push_back(folder);
	}

	DebugPrint(Log, "\n--- Loading Textures ---\n");
	int fails = 0;

	for (int i = 0; i < folders.size(); i++)
	{
		DebugPrint(Log, "\n Loading all textures within the folder '%s'\n", fm->generatePath(folders[i]).c_str());
		fails += loadAllTexturesIn(folders[i]);
	}


	DebugPrint(Log, "\n--- Texture Loading Complete: %d Failures ---\n\n", fails);
}


int TextureManager::loadAllTexturesIn(FileManager::Folder folder)
{
	int fails = 0;	
	TextureMap textureMap;
	FolderTextureMap folderMap(folder, textureMap);

	std::vector<std::string> imagePaths = FileManager::Get()->allFilesInFolder(folder);
	for (const std::string& path : imagePaths)
	{
		fails += !loadTexture(folderMap, path);
	}

	mTextures.push_back(folderMap);
	return fails;
}


bool TextureManager::loadTexture(FolderTextureMap& folderMap, const std::string& filePath)
{
	bool success = true;
	Texture *texture = new Texture;

	Renderer::Get()->Open();
	if (texture->loadFromFile(filePath))
	{
		std::string label = FileManager::Get()->getItemName(filePath);
		std::string folderPath = FileManager::Get()->generatePath(folderMap.first);
		std::string relativePath = filePath.substr(folderPath.size());

		folderMap.second[label] = texture;

		// Add to has loaded files
		LoadingManager::Get()->successfullyLoaded(filePath);

		DebugPrint(Log, "Success: loaded texture '%s' at '%s'\n", label.c_str(), relativePath.c_str());
	}
	else
	{
		std::string label = FileManager::Get()->getItemName(filePath);
		DebugPrint(Log, "Failure: texture NOT loaded '%s' at '%s'\n", label.c_str(), filePath.c_str());
		success = false;
	}

	Renderer::Get()->Close();
	return success;
}


const std::string& TextureManager::getTextureName(const Texture* texture) const
{
	for (int i = 0; i < mTextures.size(); i++)
	{
		const TextureMap& textureMap = mTextures[i].second;

		std::unordered_map<std::string, Texture*>::const_iterator iter = textureMap.begin();
		for (; iter != textureMap.end(); iter++)
		{
			if (iter->second == texture)
			{
				return iter->first;
			}
		}
	}

	ASSERT(Error, false, "No texture was found within any texture map\n");
	return std::string("");
}



Texture* TextureManager::getTexture(const std::string& label, const FileManager::Folder folder) const
{
	TextureMap textureMap = findTextureMap(folder);

	auto search = textureMap.find(label);

	if (search != textureMap.end())
	{
		return search->second;
	}
	else
	{
		DebugPrint(Warning, "No item in folder map '%d' with label: '%s'\n", folder, label.c_str());
		return nullptr;
	}
}


Texture* TextureManager::getTexture(const std::string& label) const
{
	for (int i = 0; i < mTextures.size(); i++)
	{
		TextureMap textureMap = mTextures[i].second;

		auto search = textureMap.find(label);

		if (search != textureMap.end())
		{
			return search->second;
		}
	}

	DebugPrint(Warning, "No texture with the name '%s' has been loaded into the texture map\n", label.c_str());
	return nullptr;
}


/// --- Priavte Functions --- ///

const TextureMap& TextureManager::findTextureMap(const FileManager::Folder folder) const
{
	for (int i = 0; i < mTextures.size(); i++)
	{
		if (folder == mTextures[i].first)
		{
			return mTextures[i].second;
		}
	}

	ASSERT(Error, false, "There is no texture Map in the folder '%s'\n", FileManager::Get()->generatePath(folder).c_str());
}