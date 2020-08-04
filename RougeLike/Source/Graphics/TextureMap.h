#pragma once

class Texture;

using StringTextureMap = std::unordered_map<BasicString, Texture*>;

class TextureMap
{
public:
	void free();

	void add(const BasicString& id, Texture* texture);

	Texture* texture(const BasicString& id) const { return mData.at(id); }

	BasicString find(const Texture* texture) const;
	Texture* find(const BasicString& id) const;

	int size() const { return mData.size(); }

private:
	StringTextureMap mData;
};