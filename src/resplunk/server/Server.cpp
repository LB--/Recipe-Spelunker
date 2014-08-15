#include "resplunk/server/Server.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace server;
		template<>
		Server::SpecificEvent::Registrar_t Server::SpecificEvent::Implementor_t::registrar {};
		template<>
		Server::ConstructEvent::Registrar_t Server::ConstructEvent::Implementor_t::registrar {};
		template<>
		Server::DestructEvent::Registrar_t Server::DestructEvent::Implementor_t::registrar {};
	}
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
