#ifndef resplunk_meta_Damageable_HeaderPlusPlus
#define resplunk_meta_Damageable_HeaderPlusPlus

#include "resplunk/world/Entity.hpp"

namespace resplunk
{
	namespace meta
	{
		struct Damageable
		: util::CloneImplementor<Damageable>
		{
			Damageable() noexcept;
			virtual ~Damageable() noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event() = delete;
				Event(Damageable &inst)
				: inst(inst)
				{
				}
				virtual ~Event() = 0;

				Damageable const &instance() noexcept
				{
					return inst;
				}
				Damageable &instance() const noexcept
				{
					return inst;
				}

			private:
				Damageable &inst;
			};

		protected:
			Damageable(Damageable const &from) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
		inline Damageable::Event::~Event() = default;
	}
}

#endif
