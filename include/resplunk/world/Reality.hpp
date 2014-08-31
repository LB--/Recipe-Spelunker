#ifndef resplunk_world_Reality_HeaderPlusPlus
#define resplunk_world_Reality_HeaderPlusPlus

#include "resplunk/util/Metadata.hpp"
#include "resplunk/util/Cloneable.hpp"
#include "resplunk/event/RAII.hpp"

namespace resplunk
{
	namespace world
	{
		struct Reality
		: meta::Metadatable
		, util::CloneImplementor<Reality>
		{
			using ConstructEvent = event::Construct<Reality>;
			using DestructEvent = event::Destruct<Reality>;
			Reality() noexcept;
			Reality &operator=(Reality const &) = delete;
			Reality(Reality &&) = delete;
			Reality &operator=(Reality &&) = delete;
			virtual ~Reality() noexcept;

			//

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event(Reality &inst)
				: inst(inst)
				{
				}
				virtual ~Event() = default;

				Reality const &instance() noexcept
				{
					return inst;
				}
				Reality &instance() const noexcept
				{
					return inst;
				}

			private:
				Reality &inst;
			};

		protected:
			Reality(Reality const &) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
