#define UNDEF_NEW_MACRO
#include "src/Debugging/MemLeakDetection/MemoryTracker.h"


bool memtrack::tracking = false;
unsigned long memtrack::totalAllocatedMemorySize = 0;


struct ALLOCINFO
{
	void* addr;
	unsigned long size;
	char file[128];
	unsigned int line;
};

std::list<ALLOCINFO*> allocList;

void AddTrack(void* addr, unsigned long size, const char* file, unsigned int line)
{
	if (memtrack::tracking)
	{
		ALLOCINFO* info;

		info = (ALLOCINFO*)malloc(sizeof(ALLOCINFO));
#pragma warning(push) 
#pragma warning(disable: 6011)
		info->addr = addr;
#pragma warning(pop)
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

void RemoveTrack(void* addr)
{
	if (allocList.size() > 0 && memtrack::tracking)
	{
		for (auto i = allocList.begin(); i != allocList.end(); i++)
		{
			if ((*i)->addr == addr)
			{
				memtrack::totalAllocatedMemorySize -= (*i)->size;
				free(*i);
				allocList.remove((*i));
				break;
			}
		}
	}
}


void memtrack::DumpUnfreed()
{
	if (memtrack::totalAllocatedMemorySize == 0)
		return;

	memtrack::tracking = false;
	for (auto i = allocList.begin(); i != allocList.end(); i++)
	{
		std::cerr << "UNFREED MEMORY: ADDR[0x" << (*i)->addr << "] SIZE[" << (*i)->size << " bytes] LOCATION[" << (*i)->file << " (line " << (*i)->line << ")]" << std::endl;
	}
	std::cerr << "Program exited with " << memtrack::totalAllocatedMemorySize << " unfreed bytes." << std::endl;
	allocList.clear();
}


void* operator new(size_t size, const char* file, int line)
{
	void* p = ::operator new(size);
	AddTrack(p, size, file, line);
	return p;
}

void* operator new[](size_t size, const char* file, int line)
{
	void* p = ::operator new(size);
	AddTrack(p, size, file, line);
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
	RemoveTrack(p);
	free(p);
}