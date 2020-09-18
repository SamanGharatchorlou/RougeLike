#pragma once

#include "LoadingBar.h"

class UIManager;
class Audio;

// Implementing this means the loading time will probably increase by around 10%-15%
// This was from very quick testing, may vary.
class LoadingManager
{
public:
	static LoadingManager* Get();

	void init(UIManager* UI);

	void successfullyLoaded(const BasicString& filePath);
	float loadedPercentage();

	bool end() const { return (mTotalFileSizes > 0) && (mLoadedFileSizes >= mTotalFileSizes); }

	void update();
	void render();


private:
	void CountToBeLoadedFiles();
	void directoriesToLoad(std::vector<FileManager::Folder> folders);
	void addFileToLoad(const BasicString& filePath);


private:
	LoadingManager() : mUI(nullptr), mTotalFileSizes(0), mLoadedFileSizes(0) { }
	~LoadingManager() { }

	uintmax_t mTotalFileSizes;
	uintmax_t mLoadedFileSizes;

	UIManager* mUI;

	Audio* mAudio;
};