#ifndef resplunk_world_Entity_HeaderPlusPlus
#define resplunk_world_Entity_HeaderPlusPlus

#include "resplunk/world/World.hpp"

namespace resplunk
{
	namespace world
	{
		struct Entity final
		: util::CloneImplementor<Entity>
		{
			using Location_t = util::Location<long double>;
			using ConstructEvent = event::Construct<Entity>;
			using DestructEvent = event::Destruct<Entity>;
			Entity() = delete;
			Entity(World &, Location_t const &) noexcept;
			Entity &operator=(Entity const &) = delete;
			Entity(Entity &&) = delete;
			Entity &operator=(Entity &&) = delete;
			virtual ~Entity() noexcept;

			World &world() noexcept;
			World const &world() const noexcept;

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

		private:
			Entity(Entity const &from) noexcept;

			virtual Entity *clone() const noexcept override;

			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
