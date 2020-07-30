#pragma once

class TextureManager;
class Ability;

class AbilityBuilder
{
public:
	Ability* build(const std::string& id, TextureManager* textures);


private:
	Ability* createNewAbility(const std::string& id);
	void setValues(Ability* ability, const std::string& id, const XMLParser& parser);
	void setRangedValues(Ability* ability, TextureManager* textures);
	void initAnimations(Ability* ability, const XMLParser& parser, TextureManager* textures);
};