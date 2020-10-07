#pragma once

#include <cstdint>
#include <chrono>


namespace aeth
{
	namespace UpdateCycleIntern
	{
#define UpdateFrequencies(name)	uint32_t name;
		struct UpdateFrequencies
		{
			UpdateFrequencies(render);
			UpdateFrequencies(physics);
		};
#undef UpdateFrequencies

		struct UpdateModes
		{
			bool renderFixed;
			bool physicsFixed;
		};

#define UpdateTimepoint(name)	std::chrono::steady_clock::time_point name; \
								std::chrono::steady_clock::time_point name##_last;
		struct UpdateTimepoints
		{
			UpdateTimepoint(render);
			UpdateTimepoint(physics);
		};
#undef UpdateTimepoint
	}

	class UpdateDispatcher
	{
	private:
		static bool programRunning;

	public:
		static UpdateCycleIntern::UpdateFrequencies frequencies;
		static UpdateCycleIntern::UpdateModes modes;
		static UpdateCycleIntern::UpdateTimepoints timepoints;


	public:
		static void UpdateCycle();

		static void StopRunning();
	};
	
}
