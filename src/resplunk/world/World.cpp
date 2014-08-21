#include "resplunk/world/World.hpp"

RESPLUNK_EVENT(resplunk::world::World::ConstructEvent);
RESPLUNK_EVENT(resplunk::world::World::DestructEvent);
RESPLUNK_EVENT(resplunk::world::World::Event);

namespace resplunk
{
	namespace world
	{
		struct World::Impl final
		{
			Impl() noexcept
			{
			}
			~Impl() noexcept
			{
			}

			//
		};

		World::World() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		World::~World() noexcept
		{
			DestructEvent{*this}.call();
		}
	}
}
