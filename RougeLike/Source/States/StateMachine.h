#pragma once

struct State;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void addState(State* state);
	void popState(); // do i need to delete the state??
	void replaceState(State* state);

	void processStateChanges();

	State& getActiveState();

	size_t size() { return states.size(); }

private:
	State* newState;
	std::stack<State*> states;

	bool isAdding, isRemoving, isReplacing;
};