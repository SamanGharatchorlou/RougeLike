#include "pch.h"
#include "Screen.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"

#include "UILayer.h"


Screen::Screen(GameData* gameData) : mGameData(gameData) { }


void Screen::set(std::vector<UILayer*> layers)
{
	mLayers = layers;
}


Screen::~Screen()
{
	for (UILayer* layer: mLayers)
	{
		delete layer;
	}

	mLayers.clear();
}


void Screen::render()
{
	ASSERT(Warning, mLayers.size() > 0, "Screen must have at least 1 layer\n");

	for (unsigned int i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->render();
	}	
}
