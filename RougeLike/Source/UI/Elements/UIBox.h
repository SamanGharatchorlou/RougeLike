#pragma once

#include "UIElement.h"

class Texture;

class UIBox : public UIElement
{
public:
	struct Data : public UIElement::Data
	{
		Texture* texture;
	};

public:
	UIBox() : mTexture(nullptr) { }
	UIBox(Data& data);

	void setTexture(Texture* texture) { mTexture = texture; }

	bool isPointInBounds(VectorF point);

	virtual void render();

	virtual bool isButton() const { return false; }
	virtual bool hasText() const { return false; }
	virtual Type type() const { return Type::Box; }

protected:
	Texture* mTexture;
};
