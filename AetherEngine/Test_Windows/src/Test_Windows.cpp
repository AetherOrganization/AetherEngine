#define SDL_MAIN_HANDLED
#include "src/AetherEngine.h"
#include "src/Logging/Logger.h"
#include "Src/Window/Window.h"


// Start the test from these 3 functions, not from main(). Once you merge to dev, please remove any test code and files. Thank you :)

void TestStartup()
{

}

void TestLoop()
{

}

void TestShutdown()
{

}


int main(int argc, char* argv[])
{
	Aeth::AetherEngineConfig config{};
	config.appName = "Test_Windows";

	Aeth::StartAetherEngine(argc, argv, config);
	TestStartup();

	Aeth::StartMainLoop(&TestLoop);

	TestShutdown();
	Aeth::StopAetherEngine();

	return 0;
}
