#ifndef resplunk_world_World_HeaderPlusPlus
#define resplunk_world_World_HeaderPlusPlus

#include "resplunk/event/Construct.hpp"
#include "resplunk/event/Destruct.hpp"
#include "resplunk/util/Cloneable.hpp"
#include "resplunk/util/Location.hpp"

namespace resplunk
{
	namespace world
	{
		struct World
		{
			using ConstructEvent = event::Construct<World>;
			using DestructEvent = event::Destruct<World>;
			World() noexcept;
			World(World const &) = delete;
			World &operator=(World const &) = delete;
			World(World &&) = delete;
			World &operator=(World &&) = delete;
			virtual ~World() noexcept = 0;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				virtual World const &instance() noexcept = 0;
				virtual World &instance() const noexcept = 0;
			};

			struct Inhabitant
			: util::CloneImplementor<Inhabitant>
			{
				using ConstructEvent = event::Construct<Inhabitant>;
				using DestructEvent = event::Destruct<Inhabitant>;
				using Location_t = util::Location<long double>;
				Inhabitant() = delete;
				Inhabitant(World &w, Location_t const &loc) noexcept;
				Inhabitant &operator=(Inhabitant const &) = delete;
				Inhabitant(Inhabitant &&) = delete;
				Inhabitant &operator=(Inhabitant &&) = delete;
				virtual ~Inhabitant() noexcept = 0;

				World &world() noexcept;
				World const &world() const noexcept;

				struct Event
				: event::Implementor<Event, event::Event>
				{
					Event() = default;
					virtual ~Event() = default;

					Inhabitant const &instance() noexcept
					{
						return get_instance();
					}
					Inhabitant &instance() const noexcept
					{
						return get_instance();
					}

				private:
					virtual Inhabitant &get_instance() const noexcept = 0;
				};

			protected:
				Inhabitant(Inhabitant const &) noexcept;

			private:
				struct Impl;
				std::unique_ptr<Impl> impl;
			};

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
