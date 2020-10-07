#include "UpdateCycle.h"

#include <thread>
#include <numeric>
#include <string>

#include "Src/Core/Window/Window.h"
#include "Src/Core/Ecs/RunSystems.h"
#include "Src/Math/GeneralFunctions.h"
#include "Src/Core/Logging/Logger.h"


namespace aeth
{
	std::atomic<bool> UpdateDispatcher::programRunning = false;

	UpdateCycleIntern::UpdateFrequencies UpdateDispatcher::frequencies = { 60, 40 };
	UpdateCycleIntern::UpdateModes UpdateDispatcher::modes = { false, true };
	UpdateCycleIntern::UpdateTimepoints UpdateDispatcher::timepoints = {};


	namespace
	{
		float GetDeltaTime(std::chrono::steady_clock::time_point _now, std::chrono::steady_clock::time_point _last)
		{
			return (float)(_now - _last).count() / (float)1000000000;
		}
	}


	void UpdateDispatcher::UpdateCycle()
	{
		if (programRunning)
		{
			return;
		}
		programRunning = true;


		std::chrono::steady_clock clck;
		std::chrono::steady_clock::time_point nextUpdate = clck.now();


		uint32_t* freqStrt = (uint32_t*)&frequencies; // these 2 lines automatically iterate over all the values in the UpdateFrequencies struct to find the lowest common multiple.
		uint32_t cycleFrequency = std::accumulate(freqStrt, freqStrt + (sizeof(UpdateCycleIntern::UpdateFrequencies) / sizeof(uint32_t)), 1, math::lcm);
		
		// iterate through all the values in UpdateDispatcher::timepoints and initialize them to now()
		std::chrono::steady_clock::time_point* pointsStrt = (std::chrono::steady_clock::time_point*)&timepoints;
		for (uint32_t i = 0; i < sizeof(UpdateCycleIntern::UpdateTimepoints) / sizeof(std::chrono::steady_clock::time_point); i++)
		{
			*pointsStrt = clck.now();
			pointsStrt++;
		}


		std::chrono::nanoseconds timerIncrement(1000000000 / cycleFrequency);
		std::chrono::steady_clock::time_point lastTime = clck.now();
		std::chrono::steady_clock::time_point currentTime;
		float deltaTime;

		while (programRunning)
		{
			if (clck.now() >= nextUpdate)
			{
				nextUpdate += timerIncrement;

				currentTime = clck.now();

				window::Window::UpdateEvents();

				
				while (currentTime >= timepoints.render - std::chrono::microseconds(100)) // we need to add some leve of rougness to the timing checks as the cycle loop isn't perfectly precise
				{
					if (modes.renderFixed)
					{
						ecs::RunSystems_PreRender(1.f / frequencies.render);
						ecs::RunSystems_Render(1.f / frequencies.render);
						ecs::RunSystems_PostRender(1.f / frequencies.render);

						timepoints.render += std::chrono::nanoseconds(1000000000 / frequencies.render);
					}
					else
					{
						deltaTime = GetDeltaTime(currentTime, timepoints.render_last);

						ecs::RunSystems_PreRender(deltaTime);
						ecs::RunSystems_Render(deltaTime);
						ecs::RunSystems_PostRender(deltaTime);

						timepoints.render_last = currentTime;
						timepoints.render = currentTime + std::chrono::nanoseconds(1000000000 / frequencies.render);
						break;
					}
				}	

				while (currentTime >= timepoints.physics - std::chrono::microseconds(100))
				{
					if (modes.physicsFixed)
					{
						ecs::RunSystems_PrePhysics(1.f / frequencies.physics);
						ecs::RunSystems_Physics(1.f / frequencies.physics);
						ecs::RunSystems_PostPhysics(1.f / frequencies.physics);

						timepoints.physics += std::chrono::nanoseconds(1000000000 / frequencies.physics);
					}
					else
					{
						deltaTime = GetDeltaTime(currentTime, timepoints.physics_last);

						ecs::RunSystems_PrePhysics(deltaTime);
						ecs::RunSystems_Physics(deltaTime);
						ecs::RunSystems_PostPhysics(deltaTime);

						timepoints.physics_last = currentTime;
						timepoints.physics = currentTime + std::chrono::nanoseconds(1000000000 / frequencies.physics);
						break;
					}
				}


				lastTime = currentTime;
				std::this_thread::sleep_until(nextUpdate - std::chrono::microseconds(1500));
			}
		}
	}

	void UpdateDispatcher::StopRunning()
	{
		programRunning = false;
	}
}
