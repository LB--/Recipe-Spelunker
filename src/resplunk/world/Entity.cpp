#include "resplunk/world/Entity.hpp"

#include <functional>

RESPLUNK_EVENT(resplunk::world::Entity::ConstructEvent);
RESPLUNK_EVENT(resplunk::world::Entity::DestructEvent);
RESPLUNK_EVENT(resplunk::world::Entity::Event);

namespace resplunk
{
	namespace world
	{
		struct Entity::Impl final
		{
			Impl(World &w, Location_t const &loc) noexcept
			: w{w}
			, loc{loc}
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			auto world() noexcept
			-> World &
			{
				return w;
			}
			auto world() const noexcept
			-> World const &
			{
				return w;
			}

		private:
			std::reference_wrapper<World> w;
			Location_t loc;
		};

		Entity::Entity(World &w, Location_t const &loc) noexcept
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
		-> World &
		{
			return impl->world();
		}
		auto Entity::world() const noexcept
		-> World const &
		{
			return impl->world();
		}

		auto Entity::clone() const noexcept
		-> Entity *
		{
			return new Entity{*this};
		}
	}
}
