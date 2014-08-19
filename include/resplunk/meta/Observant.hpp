#ifndef resplunk_meta_Observant_HeaderPlusPlus
#define resplunk_meta_Observant_HeaderPlusPlus

#include "resplunk/world/Entity.hpp"

namespace resplunk
{
	namespace meta
	{
		struct Observant
		: util::CloneImplementor<Observant>
		{
			Observant() noexcept;
			virtual ~Observant() noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event() = delete;
				Event(Observant &inst)
				: inst(inst)
				{
				}
				virtual ~Event() = 0;

				Observant const &instance() noexcept
				{
					return inst;
				}
				Observant &instance() const noexcept
				{
					return inst;
				}

			private:
				Observant &inst;
			};

		protected:
			Observant(Observant const &from) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
		inline Observant::Event::~Event() = default;
	}
}

#endif
