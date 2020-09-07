#pragma once

#include "ScreenLayers.h"

class Texture;
class UIElement;
class Screen;
class UISlider;
class ScreenController;

struct TexturePacket
{
	Texture* defaultTexture = nullptr;
	Texture* selected = nullptr;
	Texture* hovering = nullptr;
};


class ScreenBuilder
{
public:
	void populateScreen(Screen* screen) const;

	TexturePacket getButtonTextures(const BasicString& buttonType) const;


private:

	ScreenLayers buildUIScreen(const ScreenAttributes& screenAttributes) const;
	ScreenLayers buildScreenLayers(const ScreenAttributes& screenAttributes) const;
	ScreenLayer buildLayer(const LayerAttributes& attributes) const;
	UIElement* buildElement(const StringMap& attributes) const;
	Elements setParents(ScreenLayers& layers, const ScreenAttributes& screenAttributes) const;

	void setChildren(const ScreenLayers& screenLayers, Elements& parents) const;
	void formatElements(ScreenLayers& screenLayers) const;
	void autoSizeRect(UIElement* element) const;
	void repositionRelativeToParent(UIElement* element) const;
	void reformatText(UIElement* element) const;


private:
	Texture* getTexture(const StringMap& attributes) const;
	void buildSlider(UISlider* slider, const StringMap& attributes) const;
};
