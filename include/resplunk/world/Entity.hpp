#ifndef resplunk_world_Entity_HeaderPlusPlus
#define resplunk_world_Entity_HeaderPlusPlus

#include "resplunk/util/Metadata.hpp"
#include "resplunk/world/Reality.hpp"
#include "resplunk/util/Location.hpp"
#include "resplunk/util/Cloneable.hpp"

namespace resplunk
{
	namespace world
	{
		struct Entity
		: meta::Metadatable
		, util::CloneImplementor<Entity>
		{
			using Location_t = util::Location<long double>;
			using ConstructEvent = event::Construct<Entity>;
			using DestructEvent = event::Destruct<Entity>;
			Entity() = delete;
			Entity(Reality &, Location_t const &) noexcept;
			Entity(Entity const &from) = delete;
			Entity &operator=(Entity) = delete;
			Entity(Entity &&) = delete;
			Entity &operator=(Entity &&) = delete;
			virtual ~Entity() noexcept;

			Reality &reality() noexcept;
			Reality const &reality() const noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				virtual ~Event() noexcept = 0;

				Entity const &instance() noexcept
				{
					return inst;
				}
				Entity &instance() const noexcept
				{
					return inst;
				}

			protected:
				Event(Entity &inst)
				: inst(inst)
				{
				}

			private:
				Entity &inst;
			};

//			virtual ?...Event? new_...Event(...) = 0;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
		inline Entity::Event::~Event() noexcept = default;
	}
}

#endif
