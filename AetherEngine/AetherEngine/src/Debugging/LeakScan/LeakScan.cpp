#define UNDEF_NEW_MACRO
#include "LeakScan.h"

#ifdef _WIN32
#define strncpy strncpy_s
#endif

using namespace Aeth::Debugging;

bool LeakScan::tracking = false;
size_t LeakScan::totalAllocatedMemorySize = 0;


struct ALLOCINFO
{
	void* addr;
	size_t size;
	char file[128];
	uint32_t line;
};

std::list<ALLOCINFO*> allocList;

void AddTrack(void* addr, size_t size, const char* file, uint32_t line)
{
	if (LeakScan::tracking)
	{
		ALLOCINFO* info;

		info = (ALLOCINFO*)malloc(sizeof(ALLOCINFO));
#pragma warning(push) 
#pragma warning(disable: 6011)
		info->addr = addr;
#pragma warning(pop)
		info->size = size;
		strncpy(info->file, file, 127);
		info->line = line;

		allocList.insert(allocList.begin(), info);
		LeakScan::totalAllocatedMemorySize += size;
	}
}

void RemoveTrack(void* addr)
{
	if (allocList.size() > 0 && LeakScan::tracking)
	{
		for (auto i = allocList.begin(); i != allocList.end(); i++)
		{
			if ((*i)->addr == addr)
			{
				LeakScan::totalAllocatedMemorySize -= (*i)->size;
				free(*i);
				allocList.remove((*i));
				break;
			}
		}
	}
}


void LeakScan::DumpUnfreed()
{
	if (LeakScan::totalAllocatedMemorySize == 0)
		return;

	LeakScan::tracking = false;
	for (auto i = allocList.begin(); i != allocList.end(); i++)
	{
		std::cerr << "UNFREED MEMORY: ADDR[0x" << (*i)->addr << "] SIZE[" << (*i)->size << " bytes] LOCATION[" << (*i)->file << " (line " << (*i)->line << ")]" << std::endl;
	}
	std::cerr << "Program exited with " << LeakScan::totalAllocatedMemorySize << " unfreed bytes." << std::endl;
	allocList.clear();
}


void* operator new(size_t size, const char* file, uint32_t line)
{
	void* p = ::operator new(size);
	AddTrack(p, size, file, line);
	return p;
}

void* operator new[](size_t size, const char* file, uint32_t line)
{
	void* p = ::operator new(size);
	AddTrack(p, size, file, line);
	return p;
}

void operator delete(void* p, const char* file, uint32_t line)
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