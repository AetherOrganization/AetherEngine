#include "test.h"

#include <iostream>

void TestFunctionOnee()
{
	std::cout << "This is function one" << std::endl;
}

void TestFunctionTwo(const char* tmp)
{
	std::cout << "This is function two: " << tmp << std::endl;
}
