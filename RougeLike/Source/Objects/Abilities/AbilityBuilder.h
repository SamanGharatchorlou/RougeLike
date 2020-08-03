#pragma once

class TextureManager;
class Ability;

class AbilityBuilder
{
public:
	AbilityBuilder(TextureManager* textures) : mTextures(textures) { }

	Ability* build(const BasicString& id);


private:
	Ability* createNewAbility(const BasicString& id);
	void setValues(Ability* ability, const BasicString& id, const XMLParser& parser);
	void setRangedValues(Ability* ability);
	void initAnimations(Ability* ability, const XMLParser& parser);

private:
	TextureManager* mTextures;
};