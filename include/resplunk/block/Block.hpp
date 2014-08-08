#ifndef ResplunkBlockBlock_HeaderPlusPlus
#define ResplunkBlockBlock_HeaderPlusPlus

#include "resplunk/util/Location.hpp"

#include <cstdint>

namespace resplunk
{
	namespace block
	{
		struct Block
		{
			using Location = util::Location<std::intmax_t>;
			//
			virtual ~Block() = 0;
			//
		};
		inline Block::~Block() = default();
	}
}

#endif
