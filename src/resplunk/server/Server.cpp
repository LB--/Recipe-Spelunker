#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace server
	{
		Server::Server() noexcept
		{
			//
			ConstructEvent{*this}.call();
		}
		Server::~Server() noexcept
		{
			DestructEvent{*this}.call();
			//
		}
	}
}
