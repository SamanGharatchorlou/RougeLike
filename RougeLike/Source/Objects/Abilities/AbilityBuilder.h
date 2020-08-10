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
	void setValues(Ability* ability, const XMLNode propertiesNode);
	void setRangedValues(Ability* ability);
	void initAnimations(Ability* ability, const XMLNode animationNode);

private:
	TextureManager* mTextures;
};