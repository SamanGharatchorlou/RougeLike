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
void TextureManager::init(SDL_Renderer* setRenderer) 
{
	renderer = setRenderer;

	FileManager* fm = FileManager::Get();

	std::string config;
	std::string textureFolder;
	bool fails = 0;

	DebugPrint(Log, "\n--- Loading Textures ---\n");

	// FONTS
	//fails += !loadFont("LazyFont", fm->getFilePath(FileManager::Font, "lazy.ttf"));

	// Load all weapon textures
	fm->readFile(FileManager::Configs, "Weapons.txt", config);
	textureFolder = fm->getFolderPath(FileManager::Image_Weapons);
	fails += loadTextureConfigs(textureFolder, config);

	// load all UI bits
	fm->readFile(FileManager::Configs, "UI_Components.txt", config);
	textureFolder = fm->getFolderPath(FileManager::Image_UI);
	fails += loadTextureConfigs(textureFolder, config);

	fm->readFile(FileManager::Configs, "Map_Tiles.txt", config);
	textureFolder = fm->getFolderPath(FileManager::Image_Maps);
	fails += loadTextureConfigs(textureFolder, config);
	
	fails += !loadTexture("Soldier", fm->getFilePath(FileManager::Image_Character, "soldier.png"));
	fails += !loadTexture("Imp", fm->getFilePath(FileManager::Image_Enemies, "Imp.png"));
	fails += !loadTexture("Tileset", fm->getFilePath(FileManager::Image_Maps, "tileset.png"));

	DebugPrint(Log, " --- Texture Loading Complete - Failures: %d ---\n\n", fails);
}


// textures
bool TextureManager::loadTexture(std::string label, std::string fileName)
{
	Texture *texture = new Texture(renderer);

	if (texture->loadFromFile(fileName))
	{
		textures[label] = texture;
		DebugPrint(Log, "Sucessfully loaded texture %s as %s\n", fileName.c_str(), label.c_str());
		return true;
	}
	else
		return false;
}

//bool TextureManager::loadFont(std::string label, std::string fileName)
//{
//	Font *font = new Font(renderer);
//
//	if (font->loadFromFile(fileName))
//	{
//		fonts[label] = font;
//		DebugPrint(Log, "Sucessfully loaded font %s as %s\n", fileName.c_str(), label.c_str());
//		return true;
//	}
//	else
//		return false;
//}


// what if I put an invalid texture path???
Texture* TextureManager::getTexture(std::string label)
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

//// what if I put an invalid texture path???
//Font TextureManager::getFont(std::string label)
//{
//	auto search = fonts.find(label);
//
//	if (search != fonts.end())
//	{
//		return *(search->second);
//	}
//	else
//	{
//		DebugPrint(Warning, "No item in font map with label: %s\n", label.c_str());
//		return Font();
//	}
//}



int TextureManager::loadTextureConfigs(std::string textureFolder, std::string config)
{
	int start = 0;
	int end = 0;
	int failCounter = 0;

	for (int i = 0; i < config.length(); i++)
	{
		char character = config[i];

		if (character == '\r')
		{
			end = i;

			std::string fileName = config.substr(start, end - start);
			std::string filePath = textureFolder + fileName + ".png";

			// skip \r\n
			start = end + 2;

			// dont attempt to load blank lines
			if (fileName.empty())
				continue;

			if (fileName == "END")
				break;

			failCounter += !loadTexture(fileName, filePath);
		}
	}

	return failCounter;
}