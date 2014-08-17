#include "resplunk/entity/Observant.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace entity;
		template<>
		Observant::ConstructEvent::Registrar_t Observant::ConstructEvent::Implementor_t::registrar {};
		template<>
		Observant::DestructEvent::Registrar_t Observant::DestructEvent::Implementor_t::registrar {};
		template<>
		Observant::Event::Registrar_t Observant::Event::Implementor_t::registrar {};
	}
	namespace entity
	{
		struct Observant::Impl final
		{
			Impl() noexcept
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			//
		};

		Observant::Observant() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		Observant::Observant(Observant const &from) noexcept
		: world::Inhabitant{from}
		, impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		Observant::~Observant() noexcept
		{
			DestructEvent{*this}.call();
		}
	}
}
