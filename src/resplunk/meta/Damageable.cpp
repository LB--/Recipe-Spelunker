#include "resplunk/meta/Damageable.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace meta;
		template<>
		Damageable::Event::Registrar_t Damageable::Event::Implementor_t::registrar {};
	}
	namespace meta
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
		}
		Damageable::Damageable(Damageable const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
		}
		Damageable::~Damageable() noexcept = default;
	}
}
