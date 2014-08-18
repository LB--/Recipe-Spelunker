#ifndef resplunk_entity_Entity_HeaderPlusPlus
#define resplunk_entity_Entity_HeaderPlusPlus

#include "resplunk/world/World.hpp"

namespace resplunk
{
	namespace entity
	{
		using Location_t = util::Location<long double>;
		struct Entity
		: util::CloneImplementor<Entity>
		{
			using ConstructEvent = event::Construct<Entity>;
			using DestructEvent = event::Destruct<Entity>;
			Entity() = delete;
			Entity(world::World &, Location_t const &) noexcept;
			Entity &operator=(Entity const &) = delete;
			Entity(Entity &&) = delete;
			Entity &operator=(Entity &&) = delete;
			virtual ~Entity() noexcept;

			world::World &world() noexcept;
			world::World const &world() const noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				Entity const &instance() noexcept
				{
					return get_instance();
				}
				Entity &instance() const noexcept
				{
					return get_instance();
				}

			private:
				virtual Entity &get_instance() const noexcept = 0;
			};

		protected:
			Entity(Entity const &from) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
