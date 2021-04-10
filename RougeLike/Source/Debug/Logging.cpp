#include "pch.h"
#include "Logging.h"

#if DEBUG_CHECK
static constexpr PriorityLevel LogLevel = PriorityLevel::Log;
#else
static constexpr PriorityLevel LogLevel = PriorityLevel::Profile;
#endif

void DebugPrint(int priority, const char* format, ...)
{
#if ENABLE_LOGGING
	if (priority <= LogLevel)
	{
		va_list arg;

		va_start(arg, format);
		vfprintf(stdout, format, arg);
		va_end(arg);
	}
#endif // DEBUG_CHECK
}


void DebugAssert(int lineNumber, const char* functionName, int priority, bool condition, const char* format, ...)
{
#if DEBUG_CHECK
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
#endif // DEBUG_CHECK
}