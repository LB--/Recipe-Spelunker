#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace server
	{
		Server::Server()
		{
			//
		}
		Server::~Server()
		{
			DestructEvent(*this).call();
			//
		}
	}
}
