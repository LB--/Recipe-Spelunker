#include "resplunk/world/Inhabitant.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace world;
		template<>
		Inhabitant::ConstructEvent::Registrar_t Inhabitant::ConstructEvent::Implementor_t::registrar {};
		template<>
		Inhabitant::DestructEvent::Registrar_t Inhabitant::DestructEvent::Implementor_t::registrar {};
		template<>
		Inhabitant::Event::Registrar_t Inhabitant::Event::Implementor_t::registrar {};
	}
	namespace world
	{
		struct Inhabitant::Impl final
		{
			Impl(World &w, Location_t const &loc) noexcept
			: w(w)
			, l(loc)
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			World &world() noexcept
			{
				return w;
			}
			World const &world() const noexcept
			{
				return w;
			}

		private:
			World &w;
			Location_t l;
		};

		Inhabitant::Inhabitant(World &w, Location_t const &loc) noexcept
		: impl{new Impl{w, loc}}
		{
			ConstructEvent{*this}.call();
		}
		Inhabitant::Inhabitant(Inhabitant const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		Inhabitant::~Inhabitant() noexcept
		{
			DestructEvent{*this}.call();
		}

		auto Inhabitant::world() noexcept
		-> World &
		{
			return impl->world();
		}
		auto Inhabitant::world() const noexcept
		-> World const &
		{
			return impl->world();
		}
	}
}
