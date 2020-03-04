#include "pch.h"
#include "StateMachine.h"
#include "State.h"

StateMachine::StateMachine()
{
	states.push(new NullState);
}

StateMachine::~StateMachine()
{
	while (states.size() > 0)
	{
		states.top()->exit();
		states.pop();

		DebugPrint(Log, "State removed - %d states left on stack\n", states.size());
	}

	DebugPrint(Log, "StateMachine destroyed\n");
}

void StateMachine::addState(State* state)
{
	isAdding = true;
	isRemoving = false;
	isReplacing = false;

	newState = state;
}

// do I need to delete the state???
void StateMachine::popState()
{
	isRemoving = true;
	isReplacing = false;
	isAdding = false;
}

void StateMachine::replaceState(State* state)
{
	isReplacing = true;
	isAdding = false;
	isRemoving = false;

	newState = state;
}

void StateMachine::processStateChanges()
{
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

			delete states.top();
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
		if (states.size() > 0)
		{
			states.top()->exit();

			delete states.top();
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
}

State& StateMachine::getActiveState()
{
	return *states.top();
}