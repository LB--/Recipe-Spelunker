#include "resplunk/entity/Damageable.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace entity;
		template<>
		Damageable::ConstructEvent::Registrar_t Damageable::ConstructEvent::Implementor_t::registrar {};
		template<>
		Damageable::DestructEvent::Registrar_t Damageable::DestructEvent::Implementor_t::registrar {};
		template<>
		Damageable::Event::Registrar_t Damageable::Event::Implementor_t::registrar {};
	}
	namespace entity
	{
		struct Damageable::Impl final
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

		Damageable::Damageable() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		Damageable::Damageable(Damageable const &from) noexcept
		: world::Inhabitant{from}
		, impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		Damageable::~Damageable() noexcept
		{
			DestructEvent{*this}.call();
		}
	}
}
