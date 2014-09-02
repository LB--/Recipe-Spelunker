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

		//...
	}
}
