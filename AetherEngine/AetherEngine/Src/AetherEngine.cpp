#include "AetherEngine.h"

#include "Src/Core/Logging/Logger.h"
#include "Src/Core/Args/Args.h"
#include "Src/Core/Ecs/RunSystems.h"
#include "Src/Core/UpdateCycle.h"


namespace aeth
{
	void StartAetherEngine(int argc, char* argv[], AetherEngineConfig& _config)
	{
		Args::AddKwargHint("--verbosity", "0");
		Args::ProcessArgs(argc, argv);

		new logging::Logger("log.txt", "Main", (uint8_t)std::stoi(Args::GetKwarg("--verbosity")));

		window::Window::StartSDL();
		window::mainWindow = window::Window::StartWindow(_config.appName.data(), _config.mainWindow_XPos, _config.mainWindow_YPos, _config.mainWindow_XSize, _config.mainWindow_YSize, _config.mainWindow_Flags, UpdateDispatcher::StopRunning);
	}

	void StopAetherEngine()
	{
		window::Window::StopWindow(window::mainWindow);
		window::Window::StopSDL();

		logging::Logger::Destruct();
		Args::Cleanup();
	}

	void StartMainLoop()
	{
		UpdateDispatcher::UpdateCycle();
	}
}
