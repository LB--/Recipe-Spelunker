#include "resplunk/server/Client.hpp"

namespace resplunk
{
	namespace server
	{
		Client::Client() noexcept
		{
			//
			ConstructEvent{*this}.call();
		}
		Client::~Client() noexcept
		{
			DestructEvent{*this}.call();
			//
		}
	}
}
