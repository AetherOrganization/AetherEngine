#pragma once

#include <Src/Ecs/EntitySystemProto.h>


namespace aeth::ecs
{
	void RunSystems()
	{
		for (EntitySystemProto* proto : EntitySystemProto::systems)
		{
			proto->TestUpdate();
		}
	}
}
