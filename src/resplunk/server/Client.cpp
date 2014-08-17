#include "resplunk/server/Client.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace server;
		template<>
		Client::ConstructEvent::Registrar_t Client::ConstructEvent::Implementor_t::registrar {};
		template<>
		Client::DestructEvent::Registrar_t Client::DestructEvent::Implementor_t::registrar {};
		template<>
		Client::Event::Registrar_t Client::Event::Implementor_t::registrar {};
	}
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
