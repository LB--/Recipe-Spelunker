#ifndef resplunk_world_Entity_HeaderPlusPlus
#define resplunk_world_Entity_HeaderPlusPlus

#include "resplunk/util/Metadata.hpp"
#include "resplunk/world/Reality.hpp"
#include "resplunk/util/Location.hpp"
#include "resplunk/util/Cloneable.hpp"
#include "resplunk/event/Cancellable.hpp"

namespace resplunk
{
	namespace world
	{
		struct Entity
		: virtual util::Metadatable
		, util::CloneImplementor<Entity>
		{
			using Location_t = util::Location<long double>;
			using ConstructEvent = event::Construct<Entity>;
			using DestructEvent = event::Destruct<Entity>;
			Entity() = delete;
			Entity(Reality &, Location_t const &) noexcept;
			Entity &operator=(Entity const &) = delete;
			Entity(Entity &&) = delete;
			Entity &operator=(Entity &&) = delete;
			virtual ~Entity() noexcept;

			Reality &reality() noexcept;
			Reality const &reality() const noexcept;

			Location_t location() const noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				virtual Entity const &instance() noexcept = 0;
				virtual Entity &instance() const noexcept = 0;

				template<typename EntityT>
				using type_for = typename EntityT::Event;

			protected:
				Event() noexcept = default;
			};
			struct RealityChangeEvent
			: event::Implementor<RealityChangeEvent, event::Cancellable, Event>
			{
				Reality const &to() noexcept
				{
					return t;
				}
				Reality &to() const noexcept
				{
					return t;
				}
				void to(Reality const &r) noexcept
				{
					//const_cast required for logical-const :(
					t = const_cast<Reality &>(r);
				}

				template<typename EntityT>
				using type_for = typename EntityT::RealityChangeEvent;
				template<typename EntityT>
				friend auto make_RealityChangeEvent(EntityT &e) noexcept
				-> std::unique_ptr<type_for<EntityT>>
				{
					using EventBase = Event::type_for<EntityT>;
					using EventDerived = type_for<EntityT>;
					static_assert(std::is_base_of<Entity, EntityT>::value);
					static_assert(std::is_base_of<Event, EventBase>::value);
					static_assert(std::is_base_of<EventBase, EventDerived>::value);
					static_assert(std::is_base_of<RealityChangeEvent, EventDerived>::value);
					if(auto p = dynamic_cast<EventDerived *>(e.Entity::new_RealityChangeEvent()))
					{
						return std::unique_ptr<EventDerived>{p};
					}
					return std::unique_ptr<EventDerived>{nullptr};
				}

			protected:
				RealityChangeEvent(Reality t) noexcept
				: t{t}
				{
				}

			private:
				std::reference_wrapper<Reality> t;
			};
			struct LocationChangeEvent
			: event::Implementor<LocationChangeEvent, event::Cancellable, Event>
			{
				Location_t to() const noexcept
				{
					return t;
				}
				void to(Location_t loc) noexcept
				{
					t = loc;
				}

				template<typename EntityT>
				using type_for = typename EntityT::LocationChangeEvent;
				template<typename EntityT>
				friend auto make_LocationChangeEvent(EntityT &e) noexcept
				-> std::unique_ptr<type_for<EntityT>>
				{
					using EventBase = Event::type_for<EntityT>;
					using EventDerived = type_for<EntityT>;
					static_assert(std::is_base_of<Entity, EntityT>::value);
					static_assert(std::is_base_of<Event, EventBase>::value);
					static_assert(std::is_base_of<EventBase, EventDerived>::value);
					static_assert(std::is_base_of<LocationChangeEvent, EventDerived>::value);
					if(auto p = dynamic_cast<EventDerived *>(e.Entity::new_LocationChangeEvent()))
					{
						return std::unique_ptr<EventDerived>{p};
					}
					return std::unique_ptr<EventDerived>{nullptr};
				}

			protected:
				LocationChangeEvent(Location_t t) noexcept
				: t{t}
				{
				}

			private:
				Location_t t;
			};
			struct TeleportEvent
			: event::Implementor<TeleportEvent, RealityChangeEvent, LocationChangeEvent>
			{
				template<typename EntityT>
				using type_for = typename EntityT::TeleportEvent;
				template<typename EntityT>
				friend auto make_TeleportEvent(EntityT &e) noexcept
				-> std::unique_ptr<type_for<EntityT>>
				{
					using EventBase = Event::type_for<EntityT>;
					using EventDerived = type_for<EntityT>;
					static_assert(std::is_base_of<Entity, EntityT>::value);
					static_assert(std::is_base_of<Event, EventBase>::value);
					static_assert(std::is_base_of<EventBase, EventDerived>::value);
					static_assert(std::is_base_of<EventBase, RealityChangeEvent::type_for<EntityT>>::value);
					static_assert(std::is_base_of<EventBase, LocationChangeEvent::type_for<EntityT>>::value);
					static_assert(std::is_base_of<TeleportEvent, EventDerived>::value);
					if(auto p = dynamic_cast<EventDerived *>(e.Entity::new_TeleportEvent()))
					{
						return std::unique_ptr<EventDerived>{p};
					}
					return std::unique_ptr<EventDerived>{nullptr};
				}
			};

		protected:
			Entity(Entity const &) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;

			virtual RealityChangeEvent *new_RealityChangeEvent(Reality &to) noexcept = 0;
			virtual LocationChangeEvent *new_LocationChangeEvent(Location_t to) noexcept = 0;
			virtual TeleportEvent *new_TeleportEvent(Reality &rt, Location_t lt) noexcept = 0;
		};
	}
}

#endif
