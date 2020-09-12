#pragma once

#include "Logging/Logger.h"
#include "Args/Args.h"


namespace Aeth
{
	/// <summary>
	/// Start AetherEngine. Must be called before anything else.
	/// </summary>
	void StartAetherEngine(int argc = 0, char* argv[] = {})
	{
		Args::AddKwargHint("--verbosity", "0");
		Args::ProcessArgs(argc, argv);

		new Logging::Logger("log.txt", "Main", (uint8_t)std::stoi(Args::GetKwarg("--verbosity")));
	}

	/// <summary>
	/// Stop AetherEngine. Should be the final call to the engine.
	/// </summary>
	void StopAetherEngine()
	{
		Args::Cleanup();
		Logging::Logger::Destruct();
	}
}
