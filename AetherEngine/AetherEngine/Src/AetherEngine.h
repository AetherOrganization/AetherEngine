#pragma once

#define SDL_MAIN_HANDLED

#include <chrono>
#include <thread>

#include "Src/Core/Window/Window.h"


namespace aeth
{
	struct AetherEngineConfig
	{
		std::string appName = "";
		uint32_t mainWindow_XPos = SDL_WINDOWPOS_CENTERED;
		uint32_t mainWindow_YPos = SDL_WINDOWPOS_CENTERED;
		uint32_t mainWindow_XSize = 500;
		uint32_t mainWindow_YSize = 500;
		uint32_t mainWindow_Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	};



	/// <summary>
	/// Start AetherEngine. Must be called before anything else.
	/// </summary>
	void StartAetherEngine(int argc, char* argv[], AetherEngineConfig& _config);

	/// <summary>
	/// Stop AetherEngine. Should be the final call to the engine.
	/// </summary>
	void StopAetherEngine();

	/// <summary>
	/// Start the main engine loop. This should be called once all of your setup has been done.
	/// </summary>
	void StartMainLoop();
}
