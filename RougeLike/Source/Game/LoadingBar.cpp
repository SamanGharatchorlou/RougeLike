#include "pch.h"
#include "LoadingBar.h"

#include "Graphics/Texture.h"


LoadingBar::~LoadingBar()
{
	if (mLoadingBar)
		delete mLoadingBar;

	if (mLoadingBarContainer)
		delete mLoadingBarContainer;
}

void LoadingBar::free()
{
	delete mLoadingBar;
	mLoadingBar = nullptr;

	delete mLoadingBarContainer;
	mLoadingBarContainer = nullptr;

	mPercentage = 0.0f;
	mRect = RectF();
}

void LoadingBar::setTextures(Texture* loadingBar, Texture* loadingBarContainer)
{
	mLoadingBar = loadingBar;
	mLoadingBarContainer = loadingBarContainer;
}


void LoadingBar::render()
{
	RectF loadedRect = mRect;
	loadedRect.SetWidth(mRect.Width() * mPercentage);

	mLoadingBarContainer->render(mRect);
	mLoadingBar->render(loadedRect);
}