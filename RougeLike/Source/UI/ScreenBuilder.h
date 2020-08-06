#pragma once

#include "Screens/ScreenTypes.h"

class TextureManager;
class Texture;
class UIElement;
class Screen;


class ScreenBuilder
{
public:
	ScreenBuilder(const TextureManager* textures) : mTextures(textures) { }
	Screen* buildNewScreen(const BasicString& config);


private:
	Screen* createNewScreen(const BasicString& screenName);

	ScreenLayers buildUIScreen(const ScreenAttributes& screenAttributes);
	ScreenLayers buildScreenLayers(const ScreenAttributes& screenAttributes);
	ScreenLayer buildLayer(const LayerAttributes& attributes) const;
	UIElement* buildElement(const Attributes& attributes) const;
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

	Texture* getTexture(const Attributes& attributes) const;
	TexturePacket getButtonTextures(const Attributes& attributes) const;


private:
	const TextureManager* mTextures;
};
