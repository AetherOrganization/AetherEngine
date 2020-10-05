#define SDL_MAIN_HANDLED
#include "src/AetherEngine.h"


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
	aeth::AetherEngineConfig config{};
	config.appName = "Test_Windows";

	aeth::StartAetherEngine(argc, argv, config);
	TestStartup();

	aeth::StartMainLoop(&TestLoop);

	TestShutdown();
	aeth::StopAetherEngine();

	return 0;
}
