#pragma once
#include <string>
#include "Utilities/Rect.h"

/* Base element for all UI components */
class UIElement
{
public:
	enum class Type
	{
		None,
		Element,
		Box,
		TextBox,
		Button
	};

	struct Data
	{
		BasicString id;
		RectF rect;
		const UIElement* parent = nullptr;
		bool show = true;
	};

public:
	UIElement() : mParent(nullptr) { }
	UIElement(Data& data);

	virtual ~UIElement() { };

	const BasicString& id() const { return mId; }

	void setRect(RectF rect);
	RectF rect() const { return mRect; }

	void setParernt(UIElement* parent) { mParent = parent; }
	const UIElement* parent() const { return mParent; }

	void addChild(UIElement* child) { mChildren.push_back(child); }
	std::vector<UIElement*> children() const { return mChildren; }

	virtual Type type() const { return Type::Element; }
	virtual bool hasText() const { return false; }

	virtual void render() { };

	void setVisibility(bool visible) { mShow = visible; }
	bool isVisible() const { return mShow; }

protected:
	BasicString mId;

	RectF mRect;

	const UIElement* mParent;

	std::vector<UIElement*> mChildren;

	bool mShow;
};