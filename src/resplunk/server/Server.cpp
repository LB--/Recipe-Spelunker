#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace server;
		template<>
		Server::ConstructEvent::Registrar_t Server::ConstructEvent::Implementor_t::registrar {};
		template<>
		Server::DestructEvent::Registrar_t Server::DestructEvent::Implementor_t::registrar {};
		template<>
		Server::Event::Registrar_t Server::Event::Implementor_t::registrar {};
	}
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
