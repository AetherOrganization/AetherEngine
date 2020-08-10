#include <iostream>
#include <filesystem>

#include <Src/AetherEngine.h>
#include <Src/Logging/Logger.h>


/// <summary>
/// Start the test from in here, not from main(). Once you merge to master, please remove any test code. Thank you :)
/// </summary>
void TestStart()
{

}


int main(int argc, char* argv[])
{
	StartAetherEngine(argc, argv);
#if _MEMLEAK
	memtrack::tracking = true;
#endif

	TestStart();

	StopAetherEngine();

	return 0;
}