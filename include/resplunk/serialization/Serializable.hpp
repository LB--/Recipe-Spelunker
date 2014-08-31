#ifndef resplunk_serialization_Serializable_HeaderPlusPlus
#define resplunk_serialization_Serializable_HeaderPlusPlus

#include "resplunk/util/LiteralString.hpp"
#include "resplunk/serialization/Value.hpp"

namespace resplunk
{
	namespace serialization
	{
		struct Serializer;

		struct Serializable
		{
			virtual ~Serializable() = default;

//			static constexpr util::LiteralString SCOPE {"resplunk\0serialization\0Serializable"};

		protected:
			virtual util::Optional<ObjectValue> serialize() const noexcept = 0;

		private:
			friend Serializer;
		};
	}
}

#endif
