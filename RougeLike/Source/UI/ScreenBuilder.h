#pragma once

#include "Screens/ScreenTypes.h"

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
	Screen* buildNewScreen(const BasicString& config, ScreenController* controller);


	TexturePacket getButtonTextures(const BasicString& buttonType) const;

private:
	Screen* createNewScreen(const BasicString& screenName, ScreenController* controller);

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
