#ifndef resplunk_server_Client_HeaderPlusPlus
#define resplunk_server_Client_HeaderPlusPlus

#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace server
	{
		struct Client
		: virtual Server::Specific
		{
			using ConstructEvent = event::Construct<Client>;
			using DestructEvent = event::Destruct<Client>;
			using Specific = util::SpecificTo<Client>;
			using SpecificEvent = util::SpecificToEvent<Client>;
			Client() noexcept;
			Client(Client const &) = delete;
			Client &operator=(Client const &) = delete;
			Client(Client &&) = delete;
			Client &operator=(Client &&) = delete;
			virtual ~Client() noexcept = 0;

			friend bool operator==(Client const &a, Client const &b) noexcept
			{
				return std::addressof(a) == std::addressof(b);
			}
			friend bool operator!=(Client const &a, Client const &b) noexcept
			{
				return !(a == b);
			}

			//
		};
	}
}

#endif
