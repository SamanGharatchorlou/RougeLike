#pragma once


#include "LoadingBar.h"
#include "Events\Observer.h"

// TODO: need to add dispatchers to let the loading manager when to render i.e. update the bar
// can I somehow run this in an update function while the game data is loading??
// different threads?


// Implement this means the loading tile will probably increase by around 10%-15%
// This was from very quick testing, may vary.
class LoadingManager : public Observer
{
public:
	static LoadingManager* Get();

	void init();

	void directoriesToLoad(std::vector<FileManager::Folder> folders);

	void addFileToLoad(const std::string& filePath);
	void successfullyLoaded(const std::string& filePath);
	float loadedPercentage();

	void handleEvent(EventData& data) override;

	void render();

private:
	LoadingManager() : mTotalFileSizes(0), mLoadedFileSizes(0) { }
	~LoadingManager() { }

	uintmax_t mTotalFileSizes;
	uintmax_t mLoadedFileSizes;

	LoadingBar mLoadingBar;
	Texture* mBackground;
};