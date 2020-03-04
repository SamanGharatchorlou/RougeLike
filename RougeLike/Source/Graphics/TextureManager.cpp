#include "pch.h"
#include "TextureManager.h"

#include "Texture.h"
#include "Font.h"


TextureManager::TextureManager()
{
	DebugPrint(Log, "Texture manager created\n");
}


TextureManager::~TextureManager()
{
	//iterate through the map and delete all textures	
	std::unordered_map<std::string, Texture*>::iterator iter = textures.begin();
	for (; iter != textures.end(); iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	DebugPrint(Log, "Texture manager destroyed\n");
}


// load all textures here
void TextureManager::init() 
{
	FileManager* fm = FileManager::Get();

	std::string config;
	std::string textureFolder;
	int fails = 0;

	DebugPrint(Log, "\n--- Loading Textures ---\n");

	DebugPrint(Log, "\nWeapon textures\n");
	fails += loadAllTextures(FileManager::Image_Weapons);

	DebugPrint(Log, "\nUI Components\n");
	fails += loadAllTextures(FileManager::Image_UI);

	DebugPrint(Log, "\nMap tiles\n");
	fails += loadAllTextures(FileManager::Image_Maps);
	
	DebugPrint(Log, "\nCharacters\n");
	fails += loadAllTextures(FileManager::Image_Characters_Enemies);
	fails += loadAllTextures(FileManager::Image_Characters_Player);

	DebugPrint(Log, "\n--- Texture Loading Complete: %d Failures ---\n\n", fails);
}


int TextureManager::loadAllTextures(FileManager::Folder folder)
{
	int fails = 0;
	std::vector<std::string> imagePaths = FileManager::Get()->fullPathsInFolder(folder);

	for (const std::string& path : imagePaths)
	{
		fails += !loadTexture(FileManager::Get()->fileName(path), path);
	}

	return fails;
}


// textures
bool TextureManager::loadTexture(const std::string label, const std::string& fileName)
{
	Texture *texture = new Texture;

	if (texture->loadFromFile(fileName))
	{
		textures[label] = texture;
		DebugPrint(Log, "Success: loaded texture '%s' at %s\n", label.c_str(), fileName.c_str());
		return true;
	}
	else
		return false;
}


// what if I put an invalid texture path???
Texture* TextureManager::getTexture(const std::string& label) const
{
	auto search = textures.find(label);

	if (search != textures.end())
	{
		return search->second;
	}
	else
	{
		DebugPrint(Warning, "No item in texture map with label: %s\n", label.c_str());
		return nullptr;
	}
}
