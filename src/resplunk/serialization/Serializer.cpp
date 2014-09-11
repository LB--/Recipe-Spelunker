#include "resplunk/serialization/Serializer.hpp"
#include "resplunk/serialization/Serializable.hpp"

namespace resplunk
{
	namespace serialization
	{
		void Serializer::serialize(Serializable const &s) const noexcept
		{
			if(auto &&object = s.serialize())
			{
				object.factory_id(s.factory_id());
				serialize(object);
			}
		}
	}
}
