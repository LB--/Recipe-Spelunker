#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace server
	{
		Server::Server() noexcept
		{
			//
			Construct{*this}.call();
		}
		Server::~Server() noexcept
		{
			Destruct{*this}.call();
			//
		}
	}
}
