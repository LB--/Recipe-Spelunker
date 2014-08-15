#include "resplunk/server/Client.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace server;
		template<>
		Client::SpecificEvent::Registrar_t Client::SpecificEvent::Implementor_t::registrar {};
		template<>
		Client::ConstructEvent::Registrar_t Client::ConstructEvent::Implementor_t::registrar {};
		template<>
		Client::DestructEvent::Registrar_t Client::DestructEvent::Implementor_t::registrar {};
	}
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
