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
		Element,
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
		const UIElement* parent = nullptr;
	};

public:
	UIElement() : mParent(nullptr) { }
	UIElement(Data& data) : mId(data.id), mRect(data.rect) { } // UI TODO: net parent here

	const std::string& id() const { return mId; }

	void setRect(RectF rect);
	RectF getRect() const { return mRect; }

	virtual Type type() { return Type::Element; }
	virtual bool isButton() { return false; }
	virtual void render() { };

protected:
	std::string mId;

	RectF mRect;

	const UIElement* mParent;
};