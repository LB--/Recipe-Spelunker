#ifndef resplunk_world_Voxel_HeaderPlusPlus
#define resplunk_world_Voxel_HeaderPlusPlus

#include "resplunk/util/Metadata.hpp"
#include "resplunk/world/World.hpp"
#include "resplunk/util/Location.hpp"
#include "resplunk/util/Optional.hpp"

#include <cstdint>

namespace resplunk
{
	namespace world
	{
		struct Voxel final
		: util::Metadatable
		{
			using Location_t = util::Location<std::uintmax_t>;
			using ConstructEvent = event::Construct<Voxel>;
			using DestructEvent = event::Destruct<Voxel>;
			Voxel() = delete;
			Voxel(World &, Location_t const &) noexcept;
			Voxel(std::nullptr_t) noexcept;
			Voxel(Voxel const &from) noexcept;
			Voxel &operator=(Voxel) noexcept;
			Voxel(Voxel &&) noexcept;
			Voxel &operator=(Voxel &&) noexcept;
			~Voxel() noexcept;

			util::Optional<World &> world() noexcept;
			util::Optional<World const &> world() const noexcept;

			struct Event
			: event::Implementor<Event, event::Event>
			{
				Event(Voxel &inst)
				: inst(inst)
				{
				}
				virtual ~Event() = default;

				Voxel const &instance() noexcept
				{
					return inst;
				}
				Voxel &instance() const noexcept
				{
					return inst;
				}

			private:
				Voxel &inst;
			};

			void swap(Voxel &) noexcept;
			friend void swap(Voxel &, Voxel &) noexcept;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
	}
}

#endif
