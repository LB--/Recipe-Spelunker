#include "resplunk/player/Player.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace player;
		template<>
		Player::ConstructEvent::Registrar_t Player::ConstructEvent::Implementor_t::registrar {};
		template<>
		Player::DestructEvent::Registrar_t Player::DestructEvent::Implementor_t::registrar {};
		template<>
		Player::Event::Registrar_t Player::Event::Implementor_t::registrar {};
	}
	namespace player
	{
		struct Player::Impl final
		{
			Impl() noexcept
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			//
		};

		Player::Player() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		Player::Player(Player const &from) noexcept
		: world::Inhabitant{from}
		, impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		Player::~Player() noexcept
		{
			DestructEvent{*this}.call();
		}
	}
}
