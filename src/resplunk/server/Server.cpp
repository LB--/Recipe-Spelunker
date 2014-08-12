#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace server
	{
		Server::Server()
		{
			//
			ConstructEvent{*this}.call();
		}
		Server::~Server()
		{
			DestructEvent{*this}.call();
			//
		}
	}
}
