#ifndef resplunk_entity_Entity_HeaderPlusPlus
#define resplunk_entity_Entity_HeaderPlusPlus

#include "resplunk/world/Inhabitant.hpp"

namespace resplunk
{
	namespace entity
	{
		struct Entity
		: virtual world::Inhabitant
		, util::CloneImplementor<Entity>
		{
			using ConstructEvent = event::Construct<Entity>;
			using DestructEvent = event::Destruct<Entity>;
			Entity() noexcept;
			virtual ~Entity() noexcept;

			struct Event
			: event::Implementor<Event, world::Inhabitant::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				Entity const &instance() noexcept
				{
					return dynamic_cast<Entity const &>(world::Inhabitant::Event::instance());
				}
				Entity &instance() const noexcept
				{
					return dynamic_cast<Entity &>(world::Inhabitant::Event::instance());
				}
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
