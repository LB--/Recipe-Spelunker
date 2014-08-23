#ifndef resplunk_server_Client_HeaderPlusPlus
#define resplunk_server_Client_HeaderPlusPlus

#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace server
	{
		struct Client
		{
			using ConstructEvent = event::Construct<Client>;
			using DestructEvent = event::Destruct<Client>;
			Client() = delete;
			Client(Server &) noexcept;
			Client(Client const &) = delete;
			Client &operator=(Client const &) = delete;
			Client(Client &&) = delete;
			Client &operator=(Client &&) = delete;
			virtual ~Client() noexcept = 0;

			friend bool operator==(Client const &a, Client const &b) noexcept
			{
				return std::addressof(a) == std::addressof(b);
			}

			Server &server() noexcept;
			Server const &server() const noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				virtual Client const &instance() noexcept = 0;
				virtual Client &instance() const noexcept = 0;
			};

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
