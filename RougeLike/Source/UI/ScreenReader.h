#pragma once

#include "ScreenLayers.h"

class ScreenReader
{
public:
	const ScreenAttributes readScreen(const BasicString& screen) const;


private:
	LayerAttributes readLayer(const XMLNode node) const;

	StringMap readItemNode(const XMLNode itemNode) const;
	StringMap readtextNode(const XMLNode itemNode) const;

	bool isTextNode(const XMLNode itemNode) const;
};