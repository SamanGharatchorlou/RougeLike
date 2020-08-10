#include "pch.h"
#include "GameScreen.h"

#include "UI/Elements/UIElement.h"



GameScreen::GameScreen(const TextureManager* textures) : Screen(textures)
{ 
	//mAdditionalLayer = new ScreenLayer;

	//mScreenLayers.add());
	//mAdditionalLayer = &mScreenLayers.layers().back();

	ScreenLayer layer("HotKeys");
	add(layer);
}


void GameScreen::addElement(UIElement* element)
{
}
