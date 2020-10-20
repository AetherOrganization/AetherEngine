#pragma once

#include "Src/Core/Ecs/EntitySystemProto.h"


namespace aeth::ecs
{
	void RunSystems_PreRender(float dt);
	void RunSystems_Render(float dt);
	void RunSystems_PostRender(float dt);

	void RunSystems_PrePhysics(float dt);
	void RunSystems_Physics(float dt);
	void RunSystems_PostPhysics(float dt);
}
