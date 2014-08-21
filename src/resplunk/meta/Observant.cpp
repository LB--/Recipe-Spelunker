#include "resplunk/meta/Observant.hpp"

RESPLUNK_EVENT(resplunk::meta::Observant::Event);

namespace resplunk
{
	namespace meta
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
		}
		Observant::Observant(Observant const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
		}
		Observant::~Observant() noexcept = default;
	}
}
