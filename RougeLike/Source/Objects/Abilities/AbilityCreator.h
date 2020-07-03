#pragma once

class Ability;
class RangedAbility;
class TextureManager;

Ability* createNewAbility(const std::string& name, TextureManager* textureManager);
void setRangeCircle(RangedAbility* ability, TextureManager* textureManager);