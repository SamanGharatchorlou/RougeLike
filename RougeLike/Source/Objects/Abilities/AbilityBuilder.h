#pragma once

class TextureManager;
class Ability;
class Actor;


class AbilityBuilder
{
public:
	AbilityBuilder(TextureManager* textures, Actor* caster) : mTextures(textures), mCaster(caster) { }

	Ability* build(const BasicString& id) const;


private:
	Ability* createNewAbility(const BasicString& id) const;
	void setRangedValues(Ability* ability) const;

private:
	TextureManager* mTextures;
	Actor* mCaster;
};