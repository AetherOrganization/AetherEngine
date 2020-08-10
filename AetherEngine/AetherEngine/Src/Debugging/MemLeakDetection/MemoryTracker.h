#pragma once

#include <cstdio>
#include <cstdlib>
#include <new>
#include <iostream>
#include <list>
#include <cstring>


namespace memtrack
{
	/// <summary>
	/// While this variable is true, it will track and new/delete operations. Enable/Disable this where needed
	/// </summary>
	bool tracking = false;

	/// <summary>
	/// This is equal to the number of un-freed bytes at the current moment.
	/// </summary>
	unsigned long totalAllocatedMemorySize = 0;

	namespace intern
	{
		struct ALLOCINFO
		{
			unsigned int addr;
			unsigned int size;
			char file[128];
			unsigned int line;
		};

		std::list<ALLOCINFO*> allocList;

		void AddTrack(unsigned int addr, unsigned int size, const char* file, unsigned int line)
		{
			if (memtrack::tracking)
			{
				ALLOCINFO* info;

				info = (ALLOCINFO*)malloc(sizeof(ALLOCINFO));
				info->addr = addr;
				info->size = size;
#if _WINDOWS
				strncpy_s(info->file, file, 127);
#else
				strncpy(info->file, file, 127);
#endif
				info->line = line;

				allocList.insert(allocList.begin(), info);
				memtrack::totalAllocatedMemorySize += size;
			}
		}

		void RemoveTrack(unsigned int addr)
		{
			if (allocList.size() > 0 && memtrack::tracking)
			{
				for (auto i = allocList.begin(); i != allocList.end(); i++)
				{
					if ((*i)->addr == addr)
					{
						memtrack::totalAllocatedMemorySize -= (*i)->size;
						allocList.remove((*i));
						break;
					}
				}
			}
		}
	}

	/// <summary>
	/// This should be called at the end of the program (usually with std::atexit()). Goes through all allocated memory and prints about them.
	/// </summary>
	void DumpUnfreed()
	{
		if (memtrack::totalAllocatedMemorySize == 0)
			return;

		memtrack::tracking = false;
		for (auto i = memtrack::intern::allocList.begin(); i != memtrack::intern::allocList.end(); i++)
		{
			std::cerr << "UNFREED MEMORY: ADDR[0x" << (*i)->addr << "] SIZE[" << (*i)->size << " bytes] LOCATION[" << (*i)->file << " (line " << (*i)->line << ")]" << std::endl;
		}
		std::cerr << "Program exited with " << memtrack::totalAllocatedMemorySize << " unfreed bytes." << std::endl;
		memtrack::intern::allocList.clear();
	}
}


void* operator new(size_t size, const char* file, int line)
{
	void* p = malloc(size);
	if (p == 0)
	{
		throw new std::bad_alloc();
	}
	memtrack::intern::AddTrack((unsigned int)p, size, file, line);
	return p;
}

void operator delete(void* p, const char* file, int line)
{
	free(p);
}

void operator delete(void* p)
{
	if (p == 0)
		std::cerr << "Attempting to delete a null pointer." << std::endl;
	memtrack::intern::RemoveTrack((unsigned int)p);
	free(p);
}

#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW