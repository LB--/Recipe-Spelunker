#ifndef resplunk_server_Server_HeaderPlusPlus
#define resplunk_server_Server_HeaderPlusPlus

#include "resplunk/event/RAII.hpp"

#include <set>
#include <memory>

namespace resplunk
{
	namespace server
	{
		struct Client;

		struct Server
		{
			using ConstructEvent = event::Construct<Server>;
			using DestructEvent = event::Destruct<Server>;
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

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				virtual Server const &instance() noexcept = 0;
				virtual Server &instance() const noexcept = 0;
			};

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
