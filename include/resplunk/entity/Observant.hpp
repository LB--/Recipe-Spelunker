#ifndef resplunk_entity_Observant_HeaderPlusPlus
#define resplunk_entity_Observant_HeaderPlusPlus

#include "resplunk/entity/Entity.hpp"
#include "resplunk/world/Reality.hpp"

namespace resplunk
{
	namespace entity
	{
		struct Observant
		: virtual Entity
		, util::CloneImplementor<Observant>
		{
			using ConstructEvent = event::Construct<Observant>;
			using DestructEvent = event::Destruct<Observant>;
			Observant() noexcept;
			virtual ~Observant() noexcept;

			struct Event
			: event::Implementor<Event, Entity::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				Observant const &instance() noexcept
				{
					return dynamic_cast<Observant const &>(Entity::Event::instance());
				}
				Observant &instance() const noexcept
				{
					return dynamic_cast<Observant &>(Entity::Event::instance());
				}
			};

		protected:
			Observant(Observant const &from) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
