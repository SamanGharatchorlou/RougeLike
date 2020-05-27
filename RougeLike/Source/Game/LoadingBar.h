#pragma once

class Texture;

class LoadingBar
{
public:

	void setTextures(Texture* loadingBar, Texture* loadingBarContainer);
	void setRect(RectF rect) { mRect = rect; }

	void setPercentage(float percentage) { mPercentage = percentage; }

	void render();

private:
	float mPercentage;

	RectF mRect;
	Texture* mLoadingBar;
	Texture* mLoadingBarContainer;
};