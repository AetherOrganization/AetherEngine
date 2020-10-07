#include "Src/AetherEngine.h"
#include "Src/Core/Logging/Logger.h"
#include "Src/Core/Ecs/EntitySystemProto.h"


// Start the test from these 3 functions, not from main(). Once you merge to dev, please remove any test code and files. Thank you :)

void TestStartup()
{

}

class TestSystem : aeth::ecs::EntitySystemProto
{
	DEFINE_SYSTEM_PROTO(TestSystem);

public:

};
IMPLEMENT_SYSTEM_PROTO(TestSystem);

void TestShutdown()
{

}


int main(int argc, char* argv[])
{
	aeth::AetherEngineConfig config{};
	config.appName = "Test_Windows";

	aeth::StartAetherEngine(argc, argv, config);
	TestStartup();

	aeth::StartMainLoop();

	TestShutdown();
	aeth::StopAetherEngine();

	return 0;
}
