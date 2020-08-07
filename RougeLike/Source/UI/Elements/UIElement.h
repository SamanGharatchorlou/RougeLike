#pragma once


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

public:
	UIElement() : mParent(nullptr), mShow(false) { }
	UIElement(const Attributes& attributes);
	virtual ~UIElement() { };

	void fill(const Attributes& attributes);

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