#include "pch.h"
#include "Managers/GameController.h"

int allocationSize = 0;

void* operator new(size_t size)
{
	DebugPrint(Log, "Allocating %zu of memory\n", size);
	void* p = malloc(size);
	allocationSize += size;
	return p;
}

void operator delete(void* ptr)
{
	DebugPrint(Log, "de allocating memory\n");
	allocationSize -= sizeof(ptr);
	free(ptr);
}

void testClass()
{
	std::string id;

	id = "nothing";

	const char* anotherId = "hello here";

}

int main(int argc, char* args[])
{
	class SomeClass
	{
		int a;
		const int* b = nullptr;
		std::string id;
	};

	SomeClass a;

	testClass();

	printf("memory at %d\n", allocationSize);
	GameController GameApp("RougeLike Game");
	printf("memory at %d\n", allocationSize);
	GameApp.load();
	printf("memory at %d\n", allocationSize);
	GameApp.run();
	printf("memory at %d\n", allocationSize);

	return 0;
}


