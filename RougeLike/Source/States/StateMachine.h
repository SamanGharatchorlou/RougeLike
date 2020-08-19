#pragma once

#include <stack>

template<class T>
class StateMachine
{
public:
	StateMachine(T* nullState);
	~StateMachine();

	void shallowClear(); // leaves the base null state
	T* forcePop(); // Warning: does not call exit or resume

	void addState(T* state);
	void popState();
	void replaceState(T* state);

	T* processStateChanges();

	T& getActiveState() const;

	size_t size() const { return states.size(); }

private:
	T* newState;
	std::stack<T*> states;

	bool isAdding, isRemoving, isReplacing;
};


template<class T>
StateMachine<T>::StateMachine(T* nullState)
{
	states.push(nullState);
}


template<class T>
StateMachine<T>::~StateMachine()
{
	while (states.size() > 0)
	{
		states.top()->exit();

		delete states.top();
		states.pop();
	}
}


template<class T>
void StateMachine<T>::shallowClear()
{
	// Keep the NULL state at index 0
	while (states.size() > 1)
	{
		states.top()->exit();

		delete states.top();
		states.pop();
	}
}


template<class T>
T* StateMachine<T>::forcePop()
{
	T* state = states.top();
	states.pop();
	return state;
}


template<class T>
void StateMachine<T>::addState(T* state)
{
	isAdding = true;
	isRemoving = false;
	isReplacing = false;

	newState = state;
}


template<class T>
void StateMachine<T>::popState()
{
	isRemoving = true;
	isReplacing = false;
	isAdding = false;
}


template<class T>
void StateMachine<T>::replaceState(T* state)
{
	isReplacing = true;
	isAdding = false;
	isRemoving = false;

	newState = state;
}


template<class T>
T* StateMachine<T>::processStateChanges()
{

	T* state = nullptr;

	// add state
	if (isAdding)
	{
		states.top()->pause();

		states.push(std::move(newState));
		states.top()->init();

		isAdding = false;
	}
	// remove state
	else if (isRemoving)
	{
		if (states.size() > 1)
		{
			states.top()->exit();

			state = states.top();
			states.pop();

			states.top()->resume();
		}
		else
		{
			DebugPrint(Error, "Cannot remove state, stack size = %d\n", states.size());
		}

		isRemoving = false;
	}
	// replace state
	else if (isReplacing)
	{
		if (states.size() > 1)
		{
			states.top()->exit();

			state = states.top();
			states.pop();

			states.push(std::move(newState));
			states.top()->init();
		}
		else
		{
			DebugPrint(Error, "Cannot replace state, stack size = %d\n", states.size());
		}

		isReplacing = false;
	}

	return state;
}


template<class T>
T& StateMachine<T>::getActiveState() const
{
	return *states.top();
}
