#include "pch.h"
#include "GameScreen.h"

#include "UILayer.h"
#include "UI/Elements/UIElement.h"



GameScreen::GameScreen(const TextureManager* textures) : Screen(textures)
{ 
	mScreenLayers.add(ScreenLayer());
	mAdditionalLayer = &mScreenLayers.layers().back();
}


void GameScreen::addElement(UIElement* element)
{
	mAdditionalLayer->push_back(element);
}
