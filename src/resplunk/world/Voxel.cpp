#include "resplunk/world/Voxel.hpp"

#include <functional>

RESPLUNK_EVENT(resplunk::world::Voxel::ConstructEvent);
RESPLUNK_EVENT(resplunk::world::Voxel::DestructEvent);
RESPLUNK_EVENT(resplunk::world::Voxel::Event);

namespace resplunk
{
	namespace world
	{
		struct Voxel::Impl final
		{
			Impl(World &w, Location_t const &loc) noexcept
			: w{w}
			, loc{loc}
			{
			}
			Impl(std::nullptr_t) noexcept
			: w(nullptr)
			{
			}
			Impl(Impl const &from) = default;
			~Impl() noexcept
			{
			}

			auto world() noexcept
			-> util::Optional<World &>
			{
				return w;
			}
			auto world() const noexcept
			-> util::Optional<World const &>
			{
				return static_cast<World const &>(w);
			}

		private:
			util::Optional<World &> w;
			Location_t loc;
		};

		Voxel::Voxel(World &w, Location_t const &loc) noexcept
		: impl{new Impl{w, loc}}
		{
			ConstructEvent{*this}.call();
		}
		Voxel::Voxel(std::nullptr_t) noexcept
		: impl{new Impl{nullptr}}
		{
			ConstructEvent{*this}.call();
		}
		Voxel::Voxel(Voxel const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
			ConstructEvent{*this}.call();
		}
		auto Voxel::operator=(Voxel from) noexcept
		-> Voxel &
		{
			swap(from);
			return *this;
		}
		Voxel::Voxel(Voxel &&from) noexcept
		: impl{std::move(from.impl)}
		{
		}
		Voxel &Voxel::operator=(Voxel &&from) noexcept
		{
			impl = std::move(from.impl);
			return *this;
		}
		Voxel::~Voxel() noexcept
		{
			if(impl)
			{
				DestructEvent{*this}.call();
			}
		}

		auto Voxel::world() noexcept
		-> util::Optional<World &>
		{
			return impl->world();
		}
		auto Voxel::world() const noexcept
		-> util::Optional<World const &>
		{
			return static_cast<Impl const &>(*impl).world();
		}

		void Voxel::swap(Voxel &other) noexcept
		{
			return impl.swap(other.impl);
		}
		void swap(Voxel &a, Voxel &b) noexcept
		{
			return a.swap(b);
		}
	}
}
