#pragma once

enum PriorityLevel
{
	None = -1,
	Error,		// stop program
	Warning,	// something failed
	Log,		// basic info
	Debug		// custom debugging
};


#define ASSERT(...) DebugAssert(__LINE__, __FUNCTION__, __VA_ARGS__)


void DebugPrint(int priority, const char* format, ...); 
void DebugAssert(int lineNumber, const char* functionName, int priority, bool condition, const char* format, ...);