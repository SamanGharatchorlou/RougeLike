#include "pch.h"
#include "LoadingManager.h"

#include "Audio/AudioManager.h"
#include "Audio/Audio.h"

#include "Graphics/Renderer.h"
#include "UI/UIManager.h"
#include "UI/Screens/LoadingScreen.h"
#include "UI/Elements/UISlider.h"


LoadingManager* LoadingManager::Get()
{
	static LoadingManager sInstance;
	return &sInstance;
}


void LoadingManager::init(UIManager* UI)
{
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::FadeIn, "Menu", nullptr, 1000));
	AudioManager::Get()->handleEvents();

	CountToBeLoadedFiles();

	mUI = UI;
	mUI->controller()->addScreen(ScreenType::LoadingScreen);
	mUI->controller()->processScreenChanges();
}


void LoadingManager::update()
{
	mUI->update();
	Screen* screen = mUI->getActiveScreen();

	UISlider* loadingBar = screen->slider(ScreenItem::LoadingBar);
	float value = loadedPercentage();
	loadingBar->setSliderValue(loadedPercentage());
}


void LoadingManager::render()
{
	Renderer* renderer = Renderer::Get();
	renderer->lock();

	// Clear screen
	SDL_SetRenderDrawColor(renderer->sdlRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(renderer->sdlRenderer());

	mUI->getActiveScreen()->render();

	// Update window surface
	SDL_RenderPresent(renderer->sdlRenderer());
	renderer->unlock();
}


void LoadingManager::successfullyLoaded(const BasicString& filePath)
{
	fs::path fileSystemPath(filePath.c_str());
	if (fs::exists(fileSystemPath))
	{
		if (fs::is_directory(fileSystemPath))
		{
			std::vector<BasicString> files = FileManager::Get()->allFilesInFolder(fileSystemPath);

			for(const BasicString& file : files)
				mLoadedFileSizes += fs::file_size(fs::path(file.c_str()));
		}
		else
		{
			mLoadedFileSizes += fs::file_size(fileSystemPath);
		}
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



// -- Private Functions -- //
void LoadingManager::CountToBeLoadedFiles()
{
	std::vector<FileManager::Folder> folders;
	folders.push_back(FileManager::Images);
	folders.push_back(FileManager::Audio);

	LoadingManager::Get()->directoriesToLoad(folders);
}


void LoadingManager::directoriesToLoad(std::vector<FileManager::Folder> folders)
{
	for (FileManager::Folder folder : folders)
	{
		std::vector<BasicString> filePaths = FileManager::Get()->allFilesInFolder(folder);

		for (int i = 0; i < filePaths.size(); i++)
		{
			addFileToLoad(filePaths[i]);
		}
	}
}

void LoadingManager::addFileToLoad(const BasicString& filePath)
{
	fs::path fileSystemPath(filePath.c_str());
	if (fs::exists(fileSystemPath))
	{
		mTotalFileSizes += fs::file_size(fileSystemPath);
	}
	else
	{
		DebugPrint(Warning, "The file at path '%s' does not exist. Cannot add to 'to be loaded'\n", filePath.c_str());
	}
}