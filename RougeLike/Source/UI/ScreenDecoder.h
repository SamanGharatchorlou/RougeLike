#pragma once

#include "UI/Elements/UIButton.h"
#include "UI/Elements/UITextBox.h"

class TextureManager;
class UILayer;

typedef std::vector<UIElement*> Elements;
typedef std::vector<UILayer*> Layers;
typedef std::vector<Attributes> LayerAttributes;
typedef std::vector<std::vector<Attributes>> ScreenAttributes;



/*
Decodes an XML formatted file into a UI built into layers, boxes, buttons, text etc.
Note: The layers must be in order with the top layer first/at the top of the file
*/
class ScreenDecoder
{
public:
	ScreenDecoder(TextureManager* texManager) : tm(texManager), currentLayer(nullptr) { };
	~ScreenDecoder() { }

	ScreenAttributes getScreenAttributes(const std::string& config);
	Layers buildUIScreenLayers(ScreenAttributes& attributes);

private:
	Elements setParents(Layers layers, ScreenAttributes& screenAttributes);
	void setChildren(Elements parernts, Layers layers);
	void setRects(Layers layers);
	void setRect(UIElement* element);

	void fillElementData	(UIElement::Data& data,		Attributes& attributes) const;
	void fillBoxData		(UIBox::Data& data,			Attributes& attributes) const;
	void fillTextBoxData	(UITextBox::Data& data,		Attributes& attributes) const;
	void fillButtonData		(UIButton::Data& data,		Attributes& attributes) const;

	UIElement* findElement(Layers layers, const std::string& id);

private:
	TextureManager* tm;
	XMLParser xmlParser;

	const UILayer* currentLayer;
};
