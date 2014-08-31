#include "resplunk/serialization/Serializer.hpp"
#include "resplunk/serialization/Serializable.hpp"
#include "resplunk/serialization/Value.hpp"

namespace resplunk
{
	namespace serialization
	{
		void Serializer::serialize(Serializable const &s) const noexcept
		{
			if(auto object = s.serialize())
			{
				serialize(object);
			}
		}
	}
}
