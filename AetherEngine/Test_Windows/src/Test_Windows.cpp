#include "src/AetherEngine.h"
#include "src/Core/Logging/Logger.h"
#include "Src/Core/Ecs/EntitySystemProto.h"

#include <numeric>

#include "Src/Core/UpdateCycle.h"

USING_LOGGER;


// Start the test from these 3 functions, not from main(). Once you merge to dev, please remove any test code and files. Thank you :)

float rollingAverage[30];
uint32_t rollingI = 0;


void TestStartup()
{

}

class TestSystem : aeth::ecs::EntitySystemProto
{
	DEFINE_SYSTEM_PROTO(TestSystem);

public:
	System_Render
	{
		if (rollingI >= 30)
		{
			for (uint32_t i = 0; i < 29; i++)
			{
				rollingAverage[i] = rollingAverage[i + 1];
			}
			rollingAverage[29] = deltaTime;
		}
		else
		{
			rollingAverage[rollingI++] = deltaTime;
		}
		float value = 0;
		for (uint32_t i = 0; i < 30; i++)
		{
			value += rollingAverage[i];
		}
		Logger::Lock(Logger::DEBUG) << "Test: " << value / 30 << Logger::endl;
	}
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

	aeth::UpdateDispatcher::frequencies.render = 60;
	aeth::StartMainLoop();

	TestShutdown();
	aeth::StopAetherEngine();

	return 0;
}
