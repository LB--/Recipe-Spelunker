#include "resplunk/world/World.hpp"

namespace resplunk
{
	namespace event
	{
		using namespace world;
		template<>
		World::ConstructEvent::Registrar_t World::ConstructEvent::Implementor_t::registrar {};
		template<>
		World::DestructEvent::Registrar_t World::DestructEvent::Implementor_t::registrar {};
		template<>
		World::Event::Registrar_t World::Event::Implementor_t::registrar {};
	}
	namespace world
	{
		struct World::Impl final
		{
			Impl() noexcept
			{
			}
			~Impl() noexcept
			{
			}

			//
		};

		World::World() noexcept
		: impl{new Impl}
		{
			ConstructEvent{*this}.call();
		}
		World::~World() noexcept
		{
			DestructEvent{*this}.call();
		}
	}
}
