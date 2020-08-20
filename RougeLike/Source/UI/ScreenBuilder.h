#pragma once

#include "Screens/ScreenTypes.h"

class Texture;
class UIElement;
class Screen;
class UISlider;

struct TexturePacket
{
	Texture* defaultTexture;
	Texture* selected;
	Texture* hovering;
};


class ScreenBuilder
{
public:
	Screen* buildNewScreen(const BasicString& config);


	TexturePacket getButtonTextures(const BasicString& buttonType) const;

private:
	Screen* createNewScreen(const BasicString& screenName);

	ScreenLayers buildUIScreen(const ScreenAttributes& screenAttributes);
	ScreenLayers buildScreenLayers(const ScreenAttributes& screenAttributes);
	ScreenLayer buildLayer(const LayerAttributes& attributes) const;
	UIElement* buildElement(const StringMap& attributes) const;
	Elements setParents(ScreenLayers& layers, const ScreenAttributes& screenAttributes);

	void setChildren(const ScreenLayers& screenLayers, Elements& parents);
	void formatElements(ScreenLayers& screenLayers);
	void repositionRelativeToParent(UIElement* element) const;
	void reformatText(UIElement* element);


private:
	Texture* getTexture(const StringMap& attributes) const;

	void buildSlider(UISlider* slider, const StringMap& attributes) const;
};
