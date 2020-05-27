#include "pch.h"
#include "LoadingBar.h"

#include "Graphics/Texture.h"


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