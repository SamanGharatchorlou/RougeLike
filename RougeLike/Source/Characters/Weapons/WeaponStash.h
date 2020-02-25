#pragma once

#include "WeaponData.h"

class TextureManager;


class WeaponStash
{
public:
	WeaponStash();

	void load(TextureManager* tm);

	WeaponData& getData(std::string label);

private:
	std::unordered_map<std::string, WeaponData> data;
};