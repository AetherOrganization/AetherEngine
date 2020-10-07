
#pragma once

#include <vector>


#define DEFINE_SYSTEM_PROTO(__type__) static aeth::ecs::EntitySystemProto* EntitySystemProto_##__type__;

#define IMPLEMENT_SYSTEM_PROTO(__type__) aeth::ecs::EntitySystemProto* __type__##::EntitySystemProto_##__type__ = aeth::ecs::EntitySystemProto::AddPrototype(new __type__());


#pragma region FunctionDefines
#define System_PreRender void PreRender(float deltaTime)
#define System_Render void Render(float deltaTime)
#define System_PostRender void PostRender(float deltaTime)

#define System_PrePhysics void PrePhysics(float deltaTime)
#define System_Physics void Physics(float deltaTime)
#define System_PostPhysics void PostPhysics(float deltaTime)
#pragma endregion

namespace aeth::ecs
{
	class EntitySystemProto
	{
	public:
		static std::vector<EntitySystemProto*> systems;

		static EntitySystemProto* AddPrototype(EntitySystemProto* _p)
		{
			systems.push_back(_p);
			return _p;
		}


#pragma region UpdateFunctions
		virtual System_PreRender {};
		virtual System_Render {};
		virtual System_PostRender {};
		
		virtual System_PrePhysics {};
		virtual System_Physics {};
		virtual System_PostPhysics {};
#pragma endregion
	};
}
