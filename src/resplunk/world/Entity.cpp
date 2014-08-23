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
			Impl(Reality &r, Location_t const &loc) noexcept
			: r{r}
			, loc{loc}
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			auto reality() noexcept
			-> Reality &
			{
				return r;
			}
			auto reality() const noexcept
			-> Reality const &
			{
				return r;
			}

		private:
			std::reference_wrapper<Reality> r;
			Location_t loc;
		};

		Entity::Entity(Reality &r, Location_t const &loc) noexcept
		: impl{new Impl{r, loc}}
		{
			ConstructEvent{*this}.call();
		}
		Entity::Entity(Entity const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		auto Entity::operator=(Entity from) noexcept
		-> Entity &
		{
			swap(from);
			return *this;
		}
		Entity::Entity(Entity &&from) noexcept
		: impl{std::move(from.impl)}
		{
		}
		Entity &Entity::operator=(Entity &&from) noexcept
		{
			impl = std::move(from.impl);
			return *this;
		}
		Entity::~Entity() noexcept
		{
			if(impl)
			{
				DestructEvent{*this}.call();
			}
		}

		auto Entity::reality() noexcept
		-> Reality &
		{
			return impl->reality();
		}
		auto Entity::reality() const noexcept
		-> Reality const &
		{
			return impl->reality();
		}

		void Entity::swap(Entity &other) noexcept
		{
			return impl.swap(other.impl);
		}
		void swap(Entity &a, Entity &b) noexcept
		{
			return a.swap(b);
		}
	}
}
