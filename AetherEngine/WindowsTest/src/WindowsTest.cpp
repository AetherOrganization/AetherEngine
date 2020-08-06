#include <iostream>

#include <filesystem>

#include <Src/Dev/Test.h>
#include <Src/Logging/Logger.h>
#include <Src/Debugging/MemLeakDetection/MemoryTracker.h>

void EvalMemory()
{ // Small function that runs after main() returns. Just tells the memory tracker to look at all the unfreed memory and warn about them
	if (memtrack::totalAllocatedMemorySize != 0)
		DumpUnfreed();
}

struct SomeStruct
{
	long var1;
	int var2;
	SomeStruct* var3;
	char var4;
};

int main()
{
	std::atexit(EvalMemory); // Sets up the memory tracker to begin tracking memory and also sets up the evalmemory function to run post-main
#if _MEMLEAK
	memtrack::tracking = true;
#endif

	DoAThing();

	Logger::Log("Heyoo world! This is an example of a memory leak!", Logger::DEBUG);

	for (int i = 0; i < 10; i++)
	{
		SomeStruct* tmp = new SomeStruct();
	}

	return 0;
}