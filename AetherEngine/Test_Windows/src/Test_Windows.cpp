#define SDL_MAIN_HANDLED
#include "src/AetherEngine.h"
#include "src/Logging/Logger.h"
#include "Src/Ecs/EntitySystemProto.h"


// Start the test from these 3 functions, not from main(). Once you merge to dev, please remove any test code and files. Thank you :)

void TestStartup()
{

}

class TestSystem : aeth::ecs::EntitySystemProto
{
private:
	static aeth::ecs::EntitySystemProto* proto;


public:
	void TestUpdate()
	{

	}
};
aeth::ecs::EntitySystemProto* TestSystem::proto = aeth::ecs::EntitySystemProto::AddPrototype(new TestSystem());

void TestShutdown()
{

}


int main(int argc, char* argv[])
{
	Aeth::AetherEngineConfig config{};
	config.appName = "Test_Windows";

	Aeth::StartAetherEngine(argc, argv, config);
	TestStartup();

	Aeth::StartMainLoop();

	TestShutdown();
	Aeth::StopAetherEngine();

	return 0;
}
