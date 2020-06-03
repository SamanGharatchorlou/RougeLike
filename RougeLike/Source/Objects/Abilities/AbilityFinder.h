#pragma once

class Ability;

class AbilityFinder
{
public:
	AbilityFinder();

	Ability* get(const std::string& ability);
};