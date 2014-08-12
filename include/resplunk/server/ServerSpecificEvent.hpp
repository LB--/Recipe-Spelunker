#ifndef resplunk_server_ServerSpecificEvent_HeaderPlusPlus
#define resplunk_server_ServerSpecificEvent_HeaderPlusPlus

#include "resplunk/event/Event.hpp"
#include "resplunk/server/ServerSpecific.hpp"

namespace resplunk
{
	namespace server
	{
		struct ServerSpecificEvent
		: event::EventImplementor<ServerSpecificEvent, event::Event>
		, virtual ServerSpecific
		{
			virtual ~ServerSpecificEvent() = 0;
		};
		inline ServerSpecificEvent::~ServerSpecificEvent() = default;
	}
}

#endif
