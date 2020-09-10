#include "src/AetherEngine.h"
#include "src/Logging/Logger.h"


/// <summary>
/// Start the test from in here, not from main(). Once you merge to dev, please remove any test code and files. Thank you :)
/// </summary>
void TestStart()
{

}


int main(int argc, char* argv[])
{
	Aeth::StartAetherEngine(argc, argv);

	TestStart();

	Aeth::StopAetherEngine();

	return 0;
}
