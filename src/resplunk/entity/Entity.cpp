#include "resplunk/entity/Entity.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace entity;
		template<>
		Entity::ConstructEvent::Registrar_t Entity::ConstructEvent::Implementor_t::registrar {};
		template<>
		Entity::DestructEvent::Registrar_t Entity::DestructEvent::Implementor_t::registrar {};
		template<>
		Entity::Event::Registrar_t Entity::Event::Implementor_t::registrar {};
	}
	namespace entity
	{
		struct Entity::Impl final
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

		Entity::Entity() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		Entity::Entity(Entity const &from) noexcept
		: world::Inhabitant{from}
		, impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		Entity::~Entity() noexcept
		{
			DestructEvent{*this}.call();
		}
	}
}
