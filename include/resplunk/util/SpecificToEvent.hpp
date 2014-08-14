#ifndef resplunk_util_SpecificToEvent_HeaderPlusPlus
#define resplunk_util_SpecificToEvent_HeaderPlusPlus

#include "resplunk/event/Event.hpp"
#include "resplunk/util/SpecificTo.hpp"

namespace resplunk
{
	namespace util
	{
		template<typename T>
		struct SpecificToEvent
		: event::Implementor<SpecificToEvent<T>, event::Event>
		, virtual SpecificTo<T, true>
		{
			using SpecificTo_t = SpecificTo<T, true>;
			virtual ~SpecificToEvent() = 0;
		};
		template<typename T>
		inline SpecificToEvent<T>::~SpecificToEvent<T>() = default;
	}
}

#endif
