#pragma once

#include "Screens/ScreenTypes.h"

class Texture;
class UIElement;
class Screen;


class ScreenBuilder
{
public:
	Screen* buildNewScreen(const BasicString& config);


private:
	Screen* createNewScreen(const BasicString& screenName);

	ScreenLayers buildUIScreen(const ScreenAttributes& screenAttributes);
	ScreenLayers buildScreenLayers(const ScreenAttributes& screenAttributes);
	ScreenLayer buildLayer(const LayerAttributes& attributes) const;
	UIElement* buildElement(const StringMap& attributes) const;
	Elements setParents(ScreenLayers& layers, const ScreenAttributes& screenAttributes);

	void setChildren(const ScreenLayers& screenLayers, Elements& parents);
	void formatElements(ScreenLayers& screenLayers);
	void repositionRelativeToParent(UIElement* element);
	void reformatText(UIElement* element);


private:
	struct TexturePacket
	{
		Texture* defaultTexture;
		Texture* selected;
		Texture* hovering;
	};

	Texture* getTexture(const StringMap& attributes) const;
	TexturePacket getButtonTextures(const StringMap& attributes) const;
};
