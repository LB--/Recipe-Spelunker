#include "resplunk/world/Entity.hpp"

#include <functional>

RESPLUNK_EVENT(resplunk::world::Entity::ConstructEvent);
RESPLUNK_EVENT(resplunk::world::Entity::DestructEvent);
RESPLUNK_EVENT(resplunk::world::Entity::Event);
RESPLUNK_EVENT(resplunk::world::Entity::RealityChangeEvent);
RESPLUNK_EVENT(resplunk::world::Entity::LocationChangeEvent);
RESPLUNK_EVENT(resplunk::world::Entity::TeleportEvent);

namespace resplunk
{
	namespace world
	{
		struct Entity::Impl final
		: private event::Reactor<RealityChangeEvent>
		, private event::Reactor<LocationChangeEvent>
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

			auto location() const noexcept
			-> Location_t
			{
				return loc;
			}

		private:
			std::reference_wrapper<Reality> r;
			Location_t loc;

			virtual void react(RealityChangeEvent const &e) noexcept override
			{
				r = e.to();
			}
			virtual void react(LocationChangeEvent const &e) noexcept override
			{
				loc = e.to();
			}
		};

		Entity::Entity(Reality &r, Location_t const &loc) noexcept
		: impl{new Impl{r, loc}}
		{
			ConstructEvent{*this}.call();
		}
		Entity::Entity(Entity const &from) noexcept
		: impl{from.impl}
		{
			ConstructEvent{*this}.call();
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

		auto Entity::location() const noexcept
		-> Location_t
		{
			return impl->location();
		}
	}
}
