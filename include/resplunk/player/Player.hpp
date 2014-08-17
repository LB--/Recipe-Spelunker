#ifndef resplunk_player_Player_HeaderPlusPlus
#define resplunk_player_Player_HeaderPlusPlus

#include "resplunk/entity/Observant.hpp"
#include "resplunk/entity/Damageable.hpp"

namespace resplunk
{
	namespace player
	{
		struct Player
		: virtual entity::Observant
		, virtual entity::Damageable
		, util::CloneImplementor<Player>
		{
			using ConstructEvent = event::Construct<Player>;
			using DestructEvent = event::Destruct<Player>;
			Player() noexcept;
			virtual ~Player() noexcept;

			struct Event
			: event::Implementor<Event, entity::Observant::Event, entity::Damageable::Event>
			{
				Event() = default;
				virtual ~Event() = default;

				Player const &instance() noexcept
				{
					return dynamic_cast<Player const &>(Entity::Event::instance());
				}
				Player &instance() const noexcept
				{
					return dynamic_cast<Player &>(Entity::Event::instance());
				}
			};

		protected:
			Player(Player const &from) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
