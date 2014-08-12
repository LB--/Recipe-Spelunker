#ifndef resplunk_server_Server_HeaderPlusPlus
#define resplunk_server_Server_HeaderPlusPlus

#include "resplunk/event/ConstructEvent.hpp"
#include "resplunk/event/DestructEvent.hpp"
#include "resplunk/util/SpecificToEvent.hpp"

#include <set>

namespace resplunk
{
	namespace server
	{
		struct Server
		{
			using ConstructEvent = event::ConstructEvent<Server>;
			using DestructEvent = event::DestructEvent<Server>;
			using Specific = util::SpecificTo<Server>;
			using SpecificEvent = util::SpecificToEvent<Server>;
			Server() noexcept;
			Server(Server const &) = delete;
			Server &operator=(Server const &) = delete;
			Server(Server &&) = delete;
			Server &operator=(Server &&) = delete;
			virtual ~Server() noexcept = 0;

			friend bool operator==(Server const &a, Server const &b) noexcept
			{
				return std::addressof(a) == std::addressof(b);
			}
			friend bool operator!=(Server const &a, Server const &b) noexcept
			{
				return !(a == b);
			}

			//
		};
	}
}

#endif
