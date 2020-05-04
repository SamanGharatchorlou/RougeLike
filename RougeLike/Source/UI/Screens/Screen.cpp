#include "pch.h"
#include "Screen.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"



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


void Screen::handleInput()
{
	ASSERT(Warning, mActions.empty(), "There are %d unhandled button action(s)", mActions.size());

	for (UILayer* layer : mLayers)
	{
		layer->resetButtons();
	}

	// Apply highlighted button textures
	if (mGameData->inputManager->isCursorHeld(Cursor::Left))
	{
		for (UILayer* layer : mLayers)
		{
			layer->onPress(mGameData->inputManager->cursorPosition());
		}
	}

	// Apply button actions
	if (mGameData->inputManager->isCursorReleased(Cursor::Left))
	{
		for (UILayer* layer : mLayers)
		{
			// Get all actions for this layer
			std::queue<UIButton::Action> layerActions = layer->onRelease(mGameData->inputManager->cursorPosition());

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

	for (unsigned int i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->render();
	}	
}
