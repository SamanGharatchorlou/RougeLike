#pragma once

// TODO: this should be included in pch, why does it not work without this include?
#include "System/Files/Attributes.h"

typedef std::vector<std::vector<Attributes>> ScreenAttributes;

class TextureManager;
class UILayer;
struct GameData;


// This is only for enum
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UIText.h"
#include "UI/Elements/UITextBox.h"

/*
Decodes an XML formatted file into a UI built into layers, boxes, buttons, text etc.
Note: The layers must be in order with the top layer first/at the top of the file
*/
class ScreenDecoder
{
public:
	ScreenDecoder(GameData* gameData) : mGameData(gameData), currentLayer(nullptr) { };
	~ScreenDecoder() { }

	ScreenAttributes getScreenAttributes(std::string config);
	std::vector<UILayer*> buildUIScreenLayers(ScreenAttributes& attributes);

private:
	void fillElementData	(UIElement::Data& data,		Attributes& attributes);
	void fillBoxData		(UIBox::Data& data,			Attributes& attributes);
	void fillTextData		(UIText::Data& data,		Attributes& attributes);
	void fillButtonData		(UIButton::Data& data,		Attributes& attributes);
	void fillTextButtonData	(UITextButton::Data& data,	Attributes& attributes);

	RectF generateRect(Attributes& attributes) const;

	UIButton::Action getAction(std::string action);


private:
	GameData* mGameData;
	XMLParser xmlParser;

	const UILayer* currentLayer;
};
