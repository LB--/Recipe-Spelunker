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
		: event::EventImplementor<SpecificToEvent<T>, event::Event>
		, virtual SpecificTo<T>
		{
			using Specific_t = T;
			virtual ~SpecificToEvent() = 0;
		};
		template<typename T>
		inline SpecificToEvent<T>::~SpecificToEvent<T>() = default;
	}
}

#endif
