#include "pch.h"
#include "TextureManager.h"

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
	DebugPrint(Log, "\n--- Loading Textures ---\n");
	int fails = 0;

	//fails += loadAllTextures(FileManager::Image);

	for (int i = FileManager::Image + 1; i < FileManager::Folder::Image_END; i++)
	{
		FileManager::Folder folder = static_cast<FileManager::Folder>(i);
		DebugPrint(Log, "\n Loading all textures within the folder '%s'\n", FileManager::Get()->folderPath(folder).c_str());
		fails += loadAllTexturesIn(folder);
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
	Texture *texture = new Texture;

	if (texture->loadFromFile(filePath))
	{
		std::string label = FileManager::Get()->fileName(filePath);
		std::string folderPath = FileManager::Get()->folderPath(folderMap.first);
		std::string relativePath = filePath.substr(folderPath.size());

		folderMap.second[label] = texture;
		DebugPrint(Log, "Success: loaded texture '%s' at '%s'\n", label.c_str(), relativePath.c_str());
		return true;
	}
	else
	{
		std::string label = FileManager::Get()->fileName(filePath);
		DebugPrint(Log, "Failure: texture NOT loaded '%s' at '%s'\n", label.c_str(), filePath.c_str());
		return false;
	}

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
		DebugPrint(Warning, "No item in folder map '%d' with label: %s\n", folder, label.c_str());
		return nullptr;
	}
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