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
	// Folders to be loaded
	std::vector<FileManager::Folder> folders;
	for (int i = FileManager::Image + 1; i < FileManager::Folder::Image_END; i++)
	{
		folders.push_back(static_cast<FileManager::Folder>(i));
	}

	// Init loading (just pass int FileManager::Image folder?)
	LoadingManager::Get()->directoriesToLoad(folders);

	DebugPrint(Log, "\n--- Loading Textures ---\n");
	int fails = 0;

	for (int i = 0; i < folders.size(); i++)
	{
		DebugPrint(Log, "\n Loading all textures within the folder '%s'\n", FileManager::Get()->folderPath(folders[i]).c_str());
		fails += loadAllTexturesIn(folders[i]);
	}

	DebugPrint(Log, "\n--- Texture Loading Complete: %d Failures ---\n\n", fails);
}


int TextureManager::loadAllTexturesIn(FileManager::Folder folder)
{
	int fails = 0;	
	TextureMap textureMap;
	FolderMap folderMap(folder, textureMap);

	std::vector<std::string> imagePaths = FileManager::Get()->allFilesInFolder(folder);
	for (const std::string& path : imagePaths)
	{
		fails += !loadTexture(folderMap, path);
	}

	mTextures.push_back(folderMap);
	return fails;
}


bool TextureManager::loadTexture(FolderMap& folderMap, const std::string& filePath)
{
	bool success = true;
	Texture *texture = new Texture;

	Renderer::Get()->Open();
	if (texture->loadFromFile(filePath))
	{
		std::string label = FileManager::Get()->getFileName(filePath);
		std::string folderPath = FileManager::Get()->folderPath(folderMap.first);
		std::string relativePath = filePath.substr(folderPath.size());

		folderMap.second[label] = texture;

		// Add to has loaded files
		LoadingManager::Get()->successfullyLoaded(filePath);

		DebugPrint(Log, "Success: loaded texture '%s' at '%s'\n", label.c_str(), relativePath.c_str());
	}
	else
	{
		std::string label = FileManager::Get()->getFileName(filePath);
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

	DebugPrint(Log, "No texture was found within any texture map\n");
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


Texture* TextureManager::getTexture(const std::string& label, std::vector<FileManager::Folder> folders) const
{
	Texture* texture = nullptr;

	for (int i = 0; i < folders.size(); i++)
	{
		TextureMap textureMap = findTextureMap(folders[i]);

		auto search = textureMap.find(label);

		if (search != textureMap.end())
		{
			if(!texture)
				texture = search->second;
			else
			{
				DebugPrint(Warning, 
					"Multiple files named '%s' were found, required texture is ambiguous. \
					Either change the file names or make a searched folder more specific, \
					folders searched = %5\n", label, folders.size());
				texture = nullptr;
				break;
			}
		}
	}

	return texture;
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

	DebugPrint(Warning, "There is no texture Map in the folder '%s'\n", FileManager::Get()->folderPath(folder).c_str());
}