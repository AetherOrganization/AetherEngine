#pragma once

#include <cstdio>
#include <cstdlib>
#include <new>
#include <iostream>
#include <list>
#include <cstring>


namespace memtrack
{
	bool tracking = false;
	unsigned long totalAllocatedMemorySize = 0;

	struct ALLOCINFO
	{
		unsigned int addr;
		unsigned int size;
		char file[128];
		unsigned int line;
	};

	std::list<ALLOCINFO*> allocList;
}


void AddTrack(unsigned int addr, unsigned int size, const char* file, unsigned int line)
{
	if (memtrack::tracking)
	{
		memtrack::ALLOCINFO* info;

		info = (memtrack::ALLOCINFO*)malloc(sizeof(memtrack::ALLOCINFO));
		info->addr = addr;
		info->size = size;
#if _WINDOWS
		strncpy_s(info->file, file, 127);
#else
		//strncpy(info->file, file, 127);
#endif
		info->line = line;

		memtrack::allocList.insert(memtrack::allocList.begin(), info);
		memtrack::totalAllocatedMemorySize += size;
	}
}

void RemoveTrack(unsigned int addr)
{
	if (memtrack::allocList.size() > 0 && memtrack::tracking)
	{
		for (auto i = memtrack::allocList.begin(); i != memtrack::allocList.end(); i++)
		{
			if ((*i)->addr == addr)
			{
				memtrack::totalAllocatedMemorySize -= (*i)->size;
				memtrack::allocList.remove((*i));
				break;
			}
		}
	}
}

void* operator new(size_t size, const char* file, int line)
{
	void* p = malloc(size);
	if (p == 0)
	{
		throw new std::bad_alloc();
	}
	AddTrack((unsigned int)p, size, file, line);
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
	RemoveTrack((unsigned int)p);
	free(p);
}

void DumpUnfreed()
{
	memtrack::tracking = false;
	for (auto i = memtrack::allocList.begin(); i != memtrack::allocList.end(); i++)
	{
		std::cerr << "UNFREED MEMORY: ADDR[0x" << (*i)->addr << "] SIZE[" << (*i)->size << " bytes] LOCATION[" << (*i)->file << " (line " << (*i)->line << ")]" << std::endl;
	}
	std::cerr << "Program exited with " << memtrack::totalAllocatedMemorySize << " unfreed bytes." << std::endl;
	memtrack::allocList.clear();
}

#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW