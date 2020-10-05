#pragma once

#define SDL_MAIN_HANDLED

#include <chrono>
#include <thread>

#include "Src/Logging/Logger.h"
#include "Src/Args/Args.h"
#include "Src/Window/Window.h"


namespace aeth
{
	namespace // "Invisible" members of the namespace. Can only be accessed from this file.
	{
		bool programRunning = false;

		std::chrono::steady_clock::time_point nextFrameTime; // HACK: Just a temporary "next frame" time for a basic loop. There would eventually be various loops running at different rates.
	}

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
	void StartAetherEngine(int argc, char* argv[], AetherEngineConfig& _config)
	{
		programRunning = true;

		Args::AddKwargHint("--verbosity", "0");
		Args::ProcessArgs(argc, argv);

		new Logging::Logger("log.txt", "Main", (uint8_t)std::stoi(Args::GetKwarg("--verbosity")));

		Window::Window::StartSDL();
		Window::mainWindow = Window::Window::StartWindow(_config.appName.data(), _config.mainWindow_XPos, _config.mainWindow_YPos, _config.mainWindow_XSize, _config.mainWindow_YSize, _config.mainWindow_Flags, []() {
			programRunning = false; });
	}

	/// <summary>
	/// Stop AetherEngine. Should be the final call to the engine.
	/// </summary>
	void StopAetherEngine()
	{
		Window::Window::StopWindow(Window::mainWindow);
		Window::Window::StopSDL();
		
		Logging::Logger::Destruct();
		Args::Cleanup();
	}
	
	/// <summary>
	/// Start the main engine loop. This should be called once all of your setup has been done.
	/// </summary>
	// HACK: callbk is only used while there is no good infrastructure. Remove when possible
	void StartMainLoop(void(*callbk)())
	{
		nextFrameTime = std::chrono::steady_clock::now();

		while (programRunning)
		{
			std::this_thread::sleep_until(nextFrameTime);
			nextFrameTime = std::chrono::steady_clock::now() + std::chrono::microseconds(1000000 / 60); // 60 fps.

			Window::Window::UpdateEvents();

			callbk();
		}
	}
}
