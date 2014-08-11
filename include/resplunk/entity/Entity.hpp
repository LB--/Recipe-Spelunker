#ifndef resplunk_entity_Entity_HeaderPlusPlus
#define resplunk_entity_Entity_HeaderPlusPlus

#include "resplunk/world/World.hpp"

namespace resplunk
{
	namespace entity
	{
		struct Entity
		: world::Inhabitant
		{
			//
			virtual ~Entity() = 0;
			//
		};
		inline Entity::~Entity() = default;
	}
}

#endif
