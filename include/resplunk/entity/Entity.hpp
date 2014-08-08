#ifndef ResplunkEntityEntity_HeaderPlusPlus
#define ResplunkEntityEntity_HeaderPlusPlus

#include "resplunk/world/World.hpp"

namespace resplunk
{
	namespace entity
	{
		struct Entity : world::Inhabitant
		{
			//
			virtual ~Entity() = 0;
			//
		};
		inline Entity::~Entity() = default;
	}
}

#endif
