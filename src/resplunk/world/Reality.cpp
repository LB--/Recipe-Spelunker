#include "resplunk/world/Reality.hpp"

#include <functional>

RESPLUNK_EVENT(resplunk::world::Reality::ConstructEvent);
RESPLUNK_EVENT(resplunk::world::Reality::DestructEvent);
RESPLUNK_EVENT(resplunk::world::Reality::Event);

namespace resplunk
{
	namespace world
	{
		struct Reality::Impl final
		{
			Impl() noexcept
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			//

		private:
			//
		};

		Reality::Reality() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		Reality::Reality(Reality const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		Reality::~Reality() noexcept
		{
			if(impl)
			{
				DestructEvent{*this}.call();
			}
		}

		//
	}
}
