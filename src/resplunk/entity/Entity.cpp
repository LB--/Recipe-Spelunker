#include "resplunk/entity/Entity.hpp"

#include <functional>

namespace resplunk
{
	namespace event
	{
		using namespace entity;
		template<>
		Entity::ConstructEvent::Registrar_t Entity::ConstructEvent::Implementor_t::registrar {};
		template<>
		Entity::DestructEvent::Registrar_t Entity::DestructEvent::Implementor_t::registrar {};
		template<>
		Entity::Event::Registrar_t Entity::Event::Implementor_t::registrar {};
	}
	namespace entity
	{
		struct Entity::Impl final
		{
			Impl(world::World &w, Location_t const &loc) noexcept
			: w{w}
			, loc{loc}
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			auto world() noexcept
			-> world::World &
			{
				return w;
			}
			auto world() const noexcept
			-> world::World const &
			{
				return w;
			}

		private:
			std::reference_wrapper<world::World> w;
			Location_t loc;
		};

		Entity::Entity(world::World &w, Location_t const &loc) noexcept
		: impl{new Impl{w, loc}}
		{
			ConstructEvent{*this}.call();
		}
		Entity::Entity(Entity const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		Entity::~Entity() noexcept
		{
			DestructEvent{*this}.call();
		}

		auto Entity::world() noexcept
		-> world::World &
		{
			return impl->world();
		}
		auto Entity::world() const noexcept
		-> world::World const &
		{
			return impl->world();
		}
	}
}
