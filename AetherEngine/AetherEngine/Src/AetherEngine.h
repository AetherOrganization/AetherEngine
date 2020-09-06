#pragma once

#include <Src/Logging/Logger.h>
#include <Src/Debugging/MemLeakDetection/MemoryTracker.h>
#include <Src/Args/Args.h>


/// <summary>
/// Start AetherEngine. Must be called before anything else.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
void StartAetherEngine(int argc = 0, char* argv[] = {})
{
	std::atexit(memtrack::DumpUnfreed);

	Args::AddKwargHint("--verbosity", "0");
	Args::ProcessArgs(argc, argv);

	new Logger("Main", std::stoi(Args::GetKwarg("--verbosity")));
}

/// <summary>
/// Stop AetherEngine. Should be the final call to the engine.
/// </summary>
void StopAetherEngine()
{
	delete Logger::instance;
}