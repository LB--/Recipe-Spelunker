#ifndef resplunk_server_Server_HeaderPlusPlus
#define resplunk_server_Server_HeaderPlusPlus

#include "resplunk/event/ConstructEvent.hpp"
#include "resplunk/event/DestructEvent.hpp"

#include <set>

namespace resplunk
{
	namespace server
	{
		struct Server
		{
			using ConstructEvent = event::ConstructEvent<Server>;
			using DestructEvent = event::DestructEvent<Server>;
			Server() noexcept;
			Server(Server const &) = delete;
			Server &operator=(Server const &) = delete;
			Server(Server &&) = delete;
			Server &operator=(Server &&) = delete;
			virtual ~Server() noexcept = 0;

			//
		};
	}
}

#endif
