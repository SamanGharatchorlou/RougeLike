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

	virtual bool isButton() { return false; }
	virtual Type type() override { return Type::Box; }

protected:
	Texture* mTexture;
};
