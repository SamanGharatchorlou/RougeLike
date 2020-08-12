#pragma once

#include "LoadingBar.h"

class UITextBox;


// Implementing this means the loading time will probably increase by around 10%-15%
// This was from very quick testing, may vary.
class LoadingManager
{
public:
	static LoadingManager* Get();
	void free();

	void init();
	void CountToBeLoadedFiles();

	void directoriesToLoad(std::vector<FileManager::Folder> folders);

	void addFileToLoad(const BasicString& filePath);
	void successfullyLoaded(const BasicString& filePath);
	float loadedPercentage();

	bool end() const { return (mTotalFileSizes > 0) && (mLoadedFileSizes == mTotalFileSizes); }

	void render();


private:
	void initTextBox(VectorF screenSize);
	void setLoadingBarTextures();
	void setLoadingBarRect(VectorF screenSize);
	void setBackgroundTexture();


private:
	LoadingManager() : mBackground(nullptr), mLoadingText(nullptr), mTotalFileSizes(0), mLoadedFileSizes(0) { }
	~LoadingManager() { }

	uintmax_t mTotalFileSizes;
	uintmax_t mLoadedFileSizes;

	LoadingBar mLoadingBar;
	Texture* mBackground;
	UITextBox* mLoadingText;
};