#pragma once


class Ability;
class Actor;
class EffectPool;
class InputManager;
enum class AbilityType;
enum class AbilityState;


class AbilityHandler
{
public:
	AbilityHandler() { }

	void init(EffectPool* effectPool, std::vector<Actor*>* targets);

	Ability* get(AbilityType type);
	void add(Ability* ability);

	const std::vector<Ability*>& abilities() const { return mAbilities; }

	void clear();

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();


private:
	bool doesCollide(Ability* ability) const;
	void activateCollisions(Ability* ability) const;
	void handleState(Ability* ability, float dt);


private:
	std::vector<Actor*>* mTargets;
	EffectPool* mEffects;

	std::vector<Ability*> mAbilities;
};