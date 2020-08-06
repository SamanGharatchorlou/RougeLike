#pragma once

#include "Screens/ScreenTypes.h"


class ScreenReader
{
public:
	const ScreenAttributes readScreen(const BasicString& screen) const;


private:
	LayerAttributes readLayer(const XMLNode node) const;

	Attributes readItemNode(const XMLNode itemNode) const;
	Attributes readtextNode(const XMLNode itemNode) const;

	bool isTextNode(const XMLNode itemNode) const;
};