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
	ScreenDecoder(TextureManager* texManager) : tm(texManager) { }
	~ScreenDecoder() { }

	const ScreenAttributes getScreenAttributes(const BasicString& config);
	Layers buildUIScreenLayers(const ScreenAttributes& attributes);





private:
	Elements setParents(Layers layers, const ScreenAttributes& screenAttributes);
	void setChildren(Elements parernts, Layers layers);
	void setRects(Layers layers);
	void setRect(UIElement* element);

	void fillElementData	(UIElement::Data& data,		const Attributes& attributes) const;
	void fillBoxData		(UIBox::Data& data,			const Attributes& attributes) const;
	void fillTextBoxData	(UITextBox::Data& data,		const Attributes& attributes) const;
	void fillButtonData		(UIButton::Data& data,		const Attributes& attributes) const;

	UIElement* findElement(Layers layers, const BasicString& id);

private:
	TextureManager* tm;
	XMLParser xmlParser;
};


//class ScreenReader
//{
//public:
//	void read(const BasicString& screen);
//
//	LayerAttributes readLayer(const XMLNode node) const;
//
//	Attributes readItemNode(const XMLNode itemNode) const;
//	Attributes readtextNode(const XMLNode itemNode) const;
//
//	bool isTextNode(const XMLNode itemNode) const;
//
//private:
//
//};