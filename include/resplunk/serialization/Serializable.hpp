#ifndef resplunk_serialization_Serializable_HeaderPlusPlus
#define resplunk_serialization_Serializable_HeaderPlusPlus

#include "resplunk/util/LiteralString.hpp"

namespace resplunk
{
	namespace serialization
	{
		struct Target;
		struct Serializable
		{
			virtual ~Serializable() = default;

//			static constexpr util::LiteralString SCOPE {"resplunk\0serialization\0Serializable"};

		protected:
			virtual Target serialize() const noexcept = 0;

		private:
			friend Target;
		};
	}
}

#endif
