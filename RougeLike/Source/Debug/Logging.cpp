#include "pch.h"
#include "Logging.h"

#if _DEBUG
static constexpr PriorityLevel LogLevel = PriorityLevel::Warning;
#else
static constexpr PriorityLevel LogLevel = PriorityLevel::None;
#endif

void DebugPrint(int priority, const char* format, ...)
{
#ifdef _DEBUG
	if (priority <= LogLevel)
	{
		va_list arg;

		va_start(arg, format);
		vfprintf(stdout, format, arg);
		va_end(arg);
	}
#endif // _DEBUG
}


void DebugAssert(int lineNumber, const char* functionName, int priority, bool condition, const char* format, ...)
{
#ifdef _DEBUG
	if (priority <= LogLevel && !condition)
	{
		printf("Assert failed in %s::%d - ", functionName, lineNumber);

		va_list arg;

		va_start(arg, format);
		vfprintf(stdout, format, arg);
		va_end(arg);

		// pause the code
		int a; 
		scanf_s("%d", &a);

		// either properly pause the code or open an output window with a failed message
	}
#endif // _DEBUG
}