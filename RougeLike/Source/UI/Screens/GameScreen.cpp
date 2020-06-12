#include "pch.h"
#include "GameScreen.h"

#include "UILayer.h"
#include "UI/Elements/UIElement.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


GameScreen::GameScreen(GameData* gameData) : Screen(gameData)
{ 
	mAdditionalLayer = new UILayer;
	mLayers.push_back(mAdditionalLayer);
}


void GameScreen::addElement(UIElement* element)
{
	mAdditionalLayer->addElement(element);
}


void GameScreen::exit()
{
	std::vector<UIElement*> elements = mAdditionalLayer->elements();

	for (int i = 0; i < elements.size(); i++)
	{
		delete elements[i];
		elements[i] = nullptr;
	}

	elements.clear();
}