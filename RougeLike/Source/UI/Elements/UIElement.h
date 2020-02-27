#pragma once
#include <string>
#include "Utilities/Rect.h"

/* Base element for all UI components */
class UIElement
{
public:
	enum Type
	{
		None,
		Box,
		BasicText,
		TextBox,
		Button,
		TextButton
	};

	struct Data
	{
		std::string id;
		RectF rect;
	};

public:
	UIElement() { }
	UIElement(Data& data) : mId(data.id), mRect(data.rect) { }

	std::string id() { return mId; }

	void setRect(RectF rect) { mRect = rect; }
	RectF getRect() { return mRect; }

	virtual Type type() = 0;
	virtual bool isButton() = 0;
	virtual void render() = 0;

protected:
	std::string mId;

	RectF mRect;
};