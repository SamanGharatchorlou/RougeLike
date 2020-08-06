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


void LoadingManager::free()
{
	delete mBackground;
	mBackground = nullptr;

	delete mLoadingText;
	mLoadingText = nullptr;

	mLoadingBar.free();
}

void LoadingManager::init()
{
	VectorF screen = VectorF(1024.0f, 768.0f);

	initTextBox(screen);
	setLoadingBarTextures();
	setLoadingBarRect(screen);
	setBackgroundTexture();
}

void LoadingManager::CountToBeLoadedFiles()
{
	std::vector<FileManager::Folder> folders;
	folders.push_back(FileManager::Images);
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


// --- Private Functions --- //
void LoadingManager::initTextBox(VectorF screenSize)
{
	VectorF textPosition = VectorF(screenSize.x / 2.0f, screenSize.y / 1.35f);
	VectorF textSize = VectorF(240, 100);
	RectF textRect(VectorF(), textSize);
	textRect.SetCenter(textPosition);

	Attributes textAttributes;
	textAttributes.add("id", "loadingbar");
	textAttributes.add("text", "Loading...");
	textAttributes.add("r", "0");
	textAttributes.add("g", "0");
	textAttributes.add("b", "255");
	textAttributes.add("x", textRect.TopLeft().x);
	textAttributes.add("y", textRect.TopLeft().y);
	textAttributes.add("width", textRect.Width());
	textAttributes.add("height", textRect.Height());

	mLoadingText = new UITextBox(textAttributes);

	mLoadingText->setRect(textRect);
	mLoadingText->autoSizeFont();
}

void LoadingManager::setLoadingBarTextures()
{
	// Set textures
	BasicString loadingBar = FileManager::Get()->findFile(FileManager::PreLoadFiles, "BlueBar");
	Texture* loadingBarTexture = new Texture;
	loadingBarTexture->loadFromFile(loadingBar);

	BasicString loadingBarContainer = FileManager::Get()->findFile(FileManager::PreLoadFiles, "BlackBar");
	Texture* loadingBarContainerTexture = new Texture;
	loadingBarContainerTexture->loadFromFile(loadingBarContainer);

	mLoadingBar.setTextures(loadingBarTexture, loadingBarContainerTexture);
}

void LoadingManager::setLoadingBarRect(VectorF screenSize)
{
	VectorF barPosition = VectorF(screenSize.x / 2.0f, screenSize.y / 1.2f);
	VectorF barSize = VectorF(800.0f, 75.0f);

	RectF rect(VectorF(), barSize);
	rect.SetCenter(barPosition);

	mLoadingBar.setRect(rect);
}

void LoadingManager::setBackgroundTexture()
{
	BasicString splashScreen = FileManager::Get()->findFile(FileManager::PreLoadFiles, "SplashScreen");
	mBackground = new Texture;
	mBackground->loadFromFile(splashScreen);
}