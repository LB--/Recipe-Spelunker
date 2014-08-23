#ifndef resplunk_world_Entity_HeaderPlusPlus
#define resplunk_world_Entity_HeaderPlusPlus

#include "resplunk/meta/Metadata.hpp"
#include "resplunk/world/Reality.hpp"
#include "resplunk/util/Location.hpp"

namespace resplunk
{
	namespace world
	{
		struct Entity final
		: meta::Metadatable
		{
			using Location_t = util::Location<long double>;
			using ConstructEvent = event::Construct<Entity>;
			using DestructEvent = event::Destruct<Entity>;
			Entity() = delete;
			Entity(Reality &, Location_t const &) noexcept;
			Entity(Entity const &from) noexcept;
			Entity &operator=(Entity) noexcept;
			Entity(Entity &&) noexcept;
			Entity &operator=(Entity &&) noexcept;
			~Entity() noexcept;

			Reality &reality() noexcept;
			Reality const &reality() const noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event(Entity &inst)
				: inst(inst)
				{
				}
				virtual ~Event() = default;

				Entity const &instance() noexcept
				{
					return inst;
				}
				Entity &instance() const noexcept
				{
					return inst;
				}

			private:
				Entity &inst;
			};

			void swap(Entity &) noexcept;
			friend void swap(Entity &, Entity &) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
