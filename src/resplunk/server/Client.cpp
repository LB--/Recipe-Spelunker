#include "resplunk/server/Client.hpp"

RESPLUNK_EVENT(resplunk::server::Client::ConstructEvent);
RESPLUNK_EVENT(resplunk::server::Client::DestructEvent);
RESPLUNK_EVENT(resplunk::server::Client::Event);

namespace resplunk
{
	namespace server
	{
		struct Client::Impl final
		{
			Impl(Server &s) noexcept
			: s(s)
			{
			}
			~Impl() noexcept
			{
			}

			Server &server() noexcept
			{
				return s;
			}
			Server const &server() const noexcept
			{
				return s;
			}

		private:
			Server &s;
		};

		Client::Client(Server &s) noexcept
		: impl{new Impl{s}}
		{
			ConstructEvent{*this}.call();
		}
		Client::~Client() noexcept
		{
			DestructEvent{*this}.call();
		}

		auto Client::server() noexcept
		-> Server &
		{
			return impl->server();
		}
		auto Client::server() const noexcept
		-> Server const &
		{
			return impl->server();
		}
	}
}
