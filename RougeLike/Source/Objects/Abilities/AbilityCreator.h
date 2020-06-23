#pragma once

class Ability;
class TextureManager;

Ability* createNewAbility(const std::string& name, TextureManager* textureManager);