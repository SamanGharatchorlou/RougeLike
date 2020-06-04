#include "pch.h"
#include "LoadingManager.h"

#include "Graphics/Texture.h"
#include "UI/Elements/UITextBox.h"

#include "UI/ScreenDecoder.h"

LoadingManager* LoadingManager::Get()
{
	static LoadingManager sInstance;
	return &sInstance;
}

void LoadingManager::init()
{
	VectorF screen = VectorF(1024.0f, 768.0f);

	// Set text
	UITextBox::Data textData;
	textData.aligment = "";
	textData.font = "";
	textData.ptSize = 0;
	textData.colour = SDL_Color{ 0, 0, 255 };
	textData.texture = nullptr;
	textData.text = "Loading...";

	VectorF textPosition = VectorF(screen.x / 2.0f, screen.y / 1.35f);
	VectorF textSize = VectorF(240, 100);
	RectF textRect(VectorF(), textSize);
	textRect.SetCenter(textPosition);
	textData.rect = textRect;

	mLoadingText = new UITextBox(textData);
	mLoadingText->autoSizeFont();

	// Set textures
	std::string loadingBar = FileManager::Get()->findFile(FileManager::PreLoadFiles, "BlueBar");
	Texture* loadingBarTexture = new Texture;
	loadingBarTexture->loadFromFile(loadingBar);

	std::string loadingBarContainer = FileManager::Get()->findFile(FileManager::PreLoadFiles, "BlackBar");
	Texture* loadingBarContainerTexture = new Texture;
	loadingBarContainerTexture->loadFromFile(loadingBarContainer);

	mLoadingBar.setTextures(loadingBarTexture, loadingBarContainerTexture);

	// Set rect
	VectorF barPosition = VectorF(screen.x / 2.0f, screen.y / 1.2f);
	VectorF barSize = VectorF(800.0f, 75.0f);
	
	RectF rect(VectorF(), barSize);
	rect.SetCenter(barPosition);

	mLoadingBar.setRect(rect);
	mLoadingBar.setPercentage(0.0f);

	// Background
	std::string splashScreen = FileManager::Get()->findFile(FileManager::PreLoadFiles, "SplashScreen");
	mBackground = new Texture;
	mBackground->loadFromFile(splashScreen);
}

void LoadingManager::CountToBeLoadedFiles()
{
	std::vector<FileManager::Folder> folders;

	// Textures
	folders.push_back(FileManager::Image);

	// Audio
	folders.push_back(FileManager::Audio);

	LoadingManager::Get()->directoriesToLoad(folders);
}


void LoadingManager::render()
{
	Renderer* renderer = Renderer::Get();
	renderer->Open();

	// Clear screen
	SDL_SetRenderDrawColor(renderer->sdlRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer->sdlRenderer());

	mBackground->render(RectF(VectorF(), VectorF(1024.0f, 768.0f)));
	mLoadingText->render();
	mLoadingBar.render();

	// Update window surface
	SDL_RenderPresent(renderer->sdlRenderer());
	renderer->Close();
}


void LoadingManager::directoriesToLoad(std::vector<FileManager::Folder> folders)
{
	for (FileManager::Folder folder : folders)
	{
		std::vector<std::string> filePaths = FileManager::Get()->allFilesInFolder(folder);

		for (int i = 0; i < filePaths.size(); i++)
		{
			addFileToLoad(filePaths[i]);
		}
	}
}

void LoadingManager::addFileToLoad(const std::string& filePath)
{
	fs::path fileSystemPath(filePath);
	if (fs::exists(fileSystemPath))
	{
		mTotalFileSizes += fs::file_size(fileSystemPath);
	}
	else
	{
		DebugPrint(Warning, "The file at path '%s' does not exist. Cannot add to 'to be loaded'\n", filePath.c_str());
	}
}


void LoadingManager::successfullyLoaded(const std::string& filePath)
{
	fs::path fileSystemPath(filePath);
	if (fs::exists(fileSystemPath))
	{
		if (fs::is_directory(fileSystemPath))
		{
			std::vector<std::string> files = FileManager::Get()->allFilesInFolder(fileSystemPath);

			for(const std::string& file : files)
				mLoadedFileSizes += fs::file_size(fs::path(file));
		}
		else
			mLoadedFileSizes += fs::file_size(fileSystemPath);

		mLoadingBar.setPercentage(loadedPercentage());
	}
	else
	{
		DebugPrint(Warning, "The file at path '%s' does not exist. Cannot add to 'already loaded'\n", filePath.c_str());
	}
}

float LoadingManager::loadedPercentage()
{
	return (float)mLoadedFileSizes / (float)mTotalFileSizes;
}

