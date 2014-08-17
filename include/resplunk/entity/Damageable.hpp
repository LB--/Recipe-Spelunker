#ifndef resplunk_entity_Damageable_HeaderPlusPlus
#define resplunk_entity_Damageable_HeaderPlusPlus

#include "resplunk/entity/Entity.hpp"

namespace resplunk
{
	namespace entity
	{
		struct Damageable
		: virtual Entity
		, util::CloneImplementor<Damageable>
		{
			using ConstructEvent = event::Construct<Damageable>;
			using DestructEvent = event::Destruct<Damageable>;
			Damageable() noexcept;
			virtual ~Damageable() noexcept;

			struct Event
			: event::Implementor<Event, Entity::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				Damageable const &instance() noexcept
				{
					return dynamic_cast<Damageable const &>(Entity::Event::instance());
				}
				Damageable &instance() const noexcept
				{
					return dynamic_cast<Damageable &>(Entity::Event::instance());
				}
			};

		protected:
			Damageable(Damageable const &from) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
