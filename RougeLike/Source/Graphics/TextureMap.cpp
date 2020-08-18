#include "pch.h"
#include "TextureMap.h"


void TextureMap::free()
{
	StringTextureMap::iterator iter = mData.begin();
	for (; iter != mData.end(); iter++)
	{
		if (iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}

	mData.clear();
}



void TextureMap::add(const BasicString& id, Texture* texture)
{
#if _DEBUG
	if (mData.count(id) > 0)
	{
		DebugPrint(Warning, "ID '%s' already contained in texture map. Replacing old value, this value might be lost forever, i.e. a memory leak\n", id.c_str());
	}
#endif
	mData[id] = texture; 
}


BasicString TextureMap::find(const Texture* texture) const
{
	StringTextureMap::const_iterator iter;
	for (iter = mData.begin(); iter != mData.end(); iter++)
	{
		if (texture == iter->second)
		{
			return iter->first;
		}
	}

	return BasicString();
}


Texture* TextureMap::find(const BasicString& id) const 
{ 
 	StringTextureMap::const_iterator iter = mData.find(id);
	return iter != mData.end() ? iter->second : nullptr;
}