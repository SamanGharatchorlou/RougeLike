#pragma once

class UIElement;

using Elements = std::vector<UIElement*>;
using ScreenLayer = std::vector<UIElement*>;

using LayerAttributes = std::vector<Attributes>;
using ScreenAttributes = std::vector<LayerAttributes>;


// TODO fix whats by reference and whats by value here
struct ScreenLayers
{
	Elements elementList() const;
	ScreenLayer layer(int index) const { return mLayers[index]; }

	int size() const { return mLayers.size(); }
	void add(ScreenLayer layer);

	UIElement* find(const BasicString& id);

	std::vector<ScreenLayer>& layers() { return mLayers; }

private:
	std::vector<ScreenLayer> mLayers;
};

