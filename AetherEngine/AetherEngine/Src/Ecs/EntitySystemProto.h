#pragma once

#include <vector>


#define DEFINE_SYSTEM_PROTO(__type__) static EntitySystemProto* EntitySystemProto_##__type__;
#define IMPLEMENT_SYSTEM_PROTO(__type__) EntitySystemProto* __type__##::EntitySystemProto_##__type__ = EntitySystemProto::AddPrototype(new __type__());

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
		virtual void TestUpdate() {} // NOTE: Should be removed eventually, this is just a demonstration
#pragma endregion
	};
}
