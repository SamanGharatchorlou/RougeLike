#pragma once

class TextureManager;
class Ability;

class AbilityBuilder
{
public:
	AbilityBuilder(TextureManager* textures) : mTextures(textures) { }

	Ability* build(const std::string& id);


private:
	Ability* createNewAbility(const std::string& id);
	void setValues(Ability* ability, const std::string& id, const XMLParser& parser);
	void setRangedValues(Ability* ability);
	void initAnimations(Ability* ability, const XMLParser& parser);

private:
	TextureManager* mTextures;
};