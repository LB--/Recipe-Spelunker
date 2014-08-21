#include "resplunk/server/Server.hpp"

RESPLUNK_EVENT(resplunk::server::Server::ConstructEvent);
RESPLUNK_EVENT(resplunk::server::Server::DestructEvent);
RESPLUNK_EVENT(resplunk::server::Server::Event);

namespace resplunk
{
	namespace server
	{
		struct Server::Impl final
		{
			Impl() noexcept
			{
			}
			~Impl() noexcept
			{
			}

			//
		};

		Server::Server() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		Server::~Server() noexcept
		{
			DestructEvent{*this}.call();
		}
	}
}
