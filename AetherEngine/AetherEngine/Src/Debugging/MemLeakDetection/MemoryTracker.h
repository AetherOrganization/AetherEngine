#pragma once

#include <iostream>
#include <list>
#include <cstring>


namespace memtrack
{
	/// <summary>
	/// While this variable is true, it will track any new/delete operations within files where MemoryTracker.h is included. Enable/Disable this where needed
	/// </summary>
	extern bool tracking;

	/// <summary>
	/// This is equal to the number of un-freed bytes at the current moment.
	/// </summary>
	extern unsigned long totalAllocatedMemorySize;

	/// <summary>
	/// This should be called at the end of the program (usually with std::atexit()). Goes through all allocated memory and prints about them.
	/// </summary>
	extern void DumpUnfreed();
}


extern void* operator new(size_t size, const char* file, int line);

extern void* operator new[](size_t size, const char* file, int line);

extern void operator delete(void* p, const char* file, int line);

extern void operator delete(void* p);

#ifndef UNDEF_NEW_MACRO
#define new new(__FILE__, __LINE__)
#endif