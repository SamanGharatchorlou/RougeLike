#pragma once

class Ability;
class RangedAbility;
class TextureManager;

Ability* createNewAbility(const BasicString& name, TextureManager* textureManager);
void setRangeCircle(RangedAbility* ability, TextureManager* textureManager);