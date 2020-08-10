#pragma once

#include "UIElement.h"

class Texture;

class UIBox : public UIElement
{
public:
	struct Data : public UIElement::Data
	{
		Texture* texture = nullptr;
#if _DEBUG
		bool drawRect = false;
#endif
	};


public:
	UIBox();
	UIBox(Data& data);
	UIBox(const StringMap& attributes);
	virtual ~UIBox() { }

	void fill(const StringMap& attributes);

	void setTexture(Texture* texture) { mTexture = texture; }
	const Texture* texture() const { return mTexture; }

	bool isPointInBounds(VectorF point);

	virtual void render();

	virtual bool isButton() const { return false; }
	virtual bool hasText() const { return false; }
	virtual Type type() const { return Type::Box; }

protected:
	Texture* mTexture;

#if _DEBUG
	bool mDrawRect;
#endif
};
