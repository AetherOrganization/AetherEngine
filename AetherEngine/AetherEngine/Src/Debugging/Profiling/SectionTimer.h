#pragma once

#include <string>


namespace aeth::profiler
{	
	void StartFrame(); // special behaviour for the root node

	void StartSectionProfile(std::string _name);

	void StopSectionProfile();

	std::string GetProfileDataStr();
}
