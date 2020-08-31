#pragma once

class UIElement;

using Elements = std::vector<UIElement*>;

using LayerAttributes = std::vector<StringMap>;
using ScreenAttributes = std::vector<LayerAttributes>;


// TODO fix whats by reference and whats by value here
struct ScreenLayer
{
	ScreenLayer(const BasicString& name) : id(name) { }
	void addElements(Elements newElements) { elements.insert(elements.end(), newElements.begin(), newElements.end()); }
	const BasicString id;
	std::vector<UIElement*> elements;
};


class ScreenLayers
{
public:
	Elements elementList() const;
	ScreenLayer layer(int index) const { return mLayers[index]; }

	int size() const { return mLayers.size(); }
	void add(ScreenLayer layer);

	ScreenLayer* layer(const BasicString& id);

	UIElement* find(const BasicString& id);

	std::vector<ScreenLayer>& layers() { return mLayers; }
	const std::vector<ScreenLayer>& layers() const { return mLayers; }

private:
	std::vector<ScreenLayer> mLayers;
};

