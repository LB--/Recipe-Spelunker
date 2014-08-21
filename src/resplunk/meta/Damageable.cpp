#include "resplunk/meta/Damageable.hpp"

RESPLUNK_EVENT(resplunk::meta::Damageable::Event);

namespace resplunk
{
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
