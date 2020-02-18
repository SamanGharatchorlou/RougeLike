#include "pch.h"
#include "Screen.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"

Screen::Screen(GameData* gameData, std::vector<UILayer*> layers) :
	mGameData(gameData), mLayers(layers) { }


Screen::~Screen()
{
	for (UILayer* layer: mLayers)
	{
		delete layer;
	}

	mLayers.clear();
}


void Screen::handleInput()
{
	ASSERT(Warning, mActions.empty(), "There are %d unhandled button action(s)", mActions.size());

	if (mGameData->cursor->isPressed())
	{
		for (UILayer* layer : mLayers)
		{
			// Get all actions for this layer
			std::queue<UIButton::Action> layerActions = layer->onPress(mGameData->cursor->getPosition());

			// Merge into Screen actions (i.e. all layers)
			while (!layerActions.empty())
			{
				mActions.push(layerActions.front());
				layerActions.pop();
			}
		}
	}
}


void Screen::render()
{
	ASSERT(Warning, mLayers.size() > 0, "Screen must have at least 1 layer\n");

	for (int i = mLayers.size() - 1; i >= 0; i--)
	{
		mLayers[i]->render();
	}	
}
