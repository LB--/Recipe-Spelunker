#ifndef resplunk_world_World_HeaderPlusPlus
#define resplunk_world_World_HeaderPlusPlus

#include "resplunk/util/Metadata.hpp"
#include "resplunk/event/RAII.hpp"
#include "resplunk/world/Reality.hpp"

namespace resplunk
{
	namespace world
	{
		struct World
		: virtual util::Metadatable
		{
			using ConstructEvent = event::Construct<World>;
			using DestructEvent = event::Destruct<World>;
			World() noexcept;
			World(World const &) = delete;
			World &operator=(World const &) = delete;
			World(World &&) = delete;
			World &operator=(World &&) = delete;
			virtual ~World() noexcept;

			virtual Reality &reality() noexcept = 0;
			virtual Reality const &reality() const noexcept = 0;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				virtual World const &instance() noexcept = 0;
				virtual World &instance() const noexcept = 0;
			};

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
