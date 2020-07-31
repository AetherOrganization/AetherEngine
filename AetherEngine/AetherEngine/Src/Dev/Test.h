#pragma once

#include <Src/Logging/Logger.h>
#include <Src/Debugging/MemLeakDetection/MemoryTracker.h>

void DoAThing()
{
	new Logger("Main");
}