
#include "RunSystems.h"

#include "Src/Core/Ecs/EntitySystemProto.h"


namespace aeth::ecs
{
	void RunSystems_PreRender(float dt)
	{
		for (EntitySystemProto* proto : EntitySystemProto::systems)
		{
			proto->PreRender(dt);
		}
	}

	void RunSystems_Render(float dt)
	{
		for (EntitySystemProto* proto : EntitySystemProto::systems)
		{
			proto->Render(dt);
		}
	}

	void RunSystems_PostRender(float dt)
	{
		for (EntitySystemProto* proto : EntitySystemProto::systems)
		{
			proto->PostRender(dt);
		}
	}


	void RunSystems_PrePhysics(float dt)
	{
		for (EntitySystemProto* proto : EntitySystemProto::systems)
		{
			proto->PrePhysics(dt);
		}
	}

	void RunSystems_Physics(float dt)
	{
		for (EntitySystemProto* proto : EntitySystemProto::systems)
		{
			proto->Physics(dt);
		}
	}

	void RunSystems_PostPhysics(float dt)
	{
		for (EntitySystemProto* proto : EntitySystemProto::systems)
		{
			proto->PostPhysics(dt);
		}
	}
}
