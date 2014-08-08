#ifndef ResplunkWorldWorld_HeaderPlusPlus
#define ResplunkWorldWorld_HeaderPlusPlus

#include "resplunk/util/Location.hpp"

#include <functional>

namespace resplunk
{
	namespace world
	{
		struct World
		{
			//...

			struct Inhabitant
			{
				using Location = util::Location<long double>;
				Inhabitant(World &w, Location const &loc)
				: world(w)
				{
				}
				virtual ~Inhabitant() = 0;

				virtual World &getWorld() final
				{
					return world;
				}
				virtual World const &getWorld() const final
				{
					return world;
				}

			protected:
				virtual void setWorld(World &w) final
				{
					//
					world = w;
				}

			private:
				std::reference_wrapper<World> world;
			};

		private:
			//
		};
		inline World::Inhabitant::~Inhabitant() = default;
		using Inhabitant = World::Inhabitant;
	}
}

#endif
