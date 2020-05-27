#include "pch.h"
#include "LoadingManager.h"

#include "Graphics/Texture.h"

LoadingManager* LoadingManager::Get()
{
	static LoadingManager sInstance;
	return &sInstance;
}

void LoadingManager::init()
{
	// Set textures
	std::string loadingBar = FileManager::Get()->filePath(FileManager::PreLoadFiles, "BlueBar.png");
	Texture* loadingBarTexture = new Texture;
	loadingBarTexture->loadFromFile(loadingBar);

	std::string loadingBarContainer = FileManager::Get()->filePath(FileManager::PreLoadFiles, "BlackBar.png");
	Texture* loadingBarContainerTexture = new Texture;
	loadingBarContainerTexture->loadFromFile(loadingBarContainer);

	mLoadingBar.setTextures(loadingBarTexture, loadingBarContainerTexture);

	// Set rect
	VectorF position = VectorF(1024.0f / 2.0f, 768.0f / 1.5f);
	VectorF size = VectorF(800.0f, 100.0f);
	
	RectF rect(VectorF(), size);
	rect.SetCenter(position);

	mLoadingBar.setRect(rect);
	mLoadingBar.setPercentage(0.0f);

	// Background
	std::string splashScreen = FileManager::Get()->filePath(FileManager::PreLoadFiles, "SplashScreen.png");
	mBackground = new Texture;
	mBackground->loadFromFile(splashScreen);
}


void LoadingManager::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	mBackground->render(RectF(VectorF(), VectorF(1024.0f, 768.0f)));
	mLoadingBar.render();

	// update window surface
	SDL_RenderPresent(renderer);
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

//void LoadingManager::updateUI()
//{
//	mLoadingBar->
//}

void LoadingManager::handleEvent(EventData& data)
{
	if (data.eventType == Event::UpdateLoadingBar)
	{
		render();
	}
}