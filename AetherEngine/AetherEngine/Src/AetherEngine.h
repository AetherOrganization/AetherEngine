#pragma once

#include "Logging/Logger.h"
#include "Debugging/LeakScan/LeakScan.h"
#include "Args/Args.h"


namespace Aeth
{
	/// <summary>
	/// Start AetherEngine. Must be called before anything else.
	/// </summary>
	void StartAetherEngine(int argc = 0, char* argv[] = {})
	{
		std::atexit(Debugging::LeakScan::DumpUnfreed);

		Args::AddKwargHint("--verbosity", "0");
		Args::ProcessArgs(argc, argv);

		new Logging::Logger("Main", std::stoi(Args::GetKwarg("--verbosity")));
	}

	/// <summary>
	/// Stop AetherEngine. Should be the final call to the engine.
	/// </summary>
	void StopAetherEngine()
	{
		delete Logging::Logger::instance;
	}
}
