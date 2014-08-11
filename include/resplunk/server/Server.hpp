#ifndef resplunk_server_Server_HeaderPlusPlus
#define resplunk_server_Server_HeaderPlusPlus

#include "resplunk/event/DestructEvent.hpp"

#include <set>

namespace resplunk
{
	namespace server
	{
		struct Server
		{
			Server();
			Server(Server const &) = delete;
			Server &operator=(Server const &) = delete;
			Server(Server &&) = delete;
			Server &operator=(Server &&) = delete;
			using DestructEvent = event::DestructEvent<Server>;
			virtual ~Server();

			//
		};
	}
}

#endif
