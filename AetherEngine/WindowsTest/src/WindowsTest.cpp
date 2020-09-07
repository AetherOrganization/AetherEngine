#include <iostream>
#include <filesystem>

#include "../../AetherEngine/Src/AetherEngine.h"
#include "../../AetherEngine/Src/Logging/Logger.h"


/// <summary>
/// Start the test from in here, not from main(). Once you merge to master, please remove any test code. Thank you :)
/// </summary>
void TestStart()
{

}


int main(int argc, char* argv[])
{
#if _MEMLEAK
	memtrack::tracking = true;
#endif
	StartAetherEngine(argc, argv);

	TestStart();

	StopAetherEngine();

	return 0;
}