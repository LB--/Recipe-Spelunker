#include "resplunk/meta/Observant.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace meta;
		template<>
		Observant::Event::Registrar_t Observant::Event::Implementor_t::registrar {};
	}
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
