#include "resplunk/server/Client.hpp"

namespace resplunk
{
	namespace server
	{
		Client::Client() noexcept
		{
			//
			Construct{*this}.call();
		}
		Client::~Client() noexcept
		{
			Destruct{*this}.call();
			//
		}
	}
}
