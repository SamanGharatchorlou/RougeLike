#pragma once

enum PriorityLevel
{
	None = -1,
	Error,		// stop program
	Warning,	// something failed
	Profile,	// used when profiling
	Log			// basic info
};


#if DEBUG_CHECK
#define ASSERT(...) DebugAssert(__LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define ASSERT(...)
#endif


void DebugPrint(int priority, const char* format, ...); 
void DebugAssert(int lineNumber, const char* functionName, int priority, bool condition, const char* format, ...);