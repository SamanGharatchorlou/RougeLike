#pragma once

#include "LoadingBar.h"

class UITextBox;


// Implementing this means the loading time will probably increase by around 10%-15%
// This was from very quick testing, may vary.
class LoadingManager
{
public:
	static LoadingManager* Get();

	void init();
	void CountToBeLoadedFiles();

	void directoriesToLoad(std::vector<FileManager::Folder> folders);

	void addFileToLoad(const BasicString& filePath);
	void successfullyLoaded(const BasicString& filePath);
	float loadedPercentage();

	bool end() const { return (mTotalFileSizes > 0) && (mLoadedFileSizes == mTotalFileSizes); }

	void render();

private:
	LoadingManager() : mTotalFileSizes(0), mLoadedFileSizes(0) { }
	~LoadingManager() { }

	uintmax_t mTotalFileSizes;
	uintmax_t mLoadedFileSizes;

	UITextBox* mLoadingText;
	LoadingBar mLoadingBar;
	Texture* mBackground;
};