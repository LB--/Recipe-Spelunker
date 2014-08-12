#ifndef resplunk_world_World_HeaderPlusPlus
#define resplunk_world_World_HeaderPlusPlus

#include "resplunk/util/Cloneable.hpp"
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
			: CloneImplementor<Inhabitant>
			{
				using Location = util::Location<long double>;
				Inhabitant(World &w, Location const &loc)
				: w(w)
				//
				{
				}
				virtual ~Inhabitant() = 0;

				virtual World &world() final
				{
					return w;
				}
				virtual World const &world() const final
				{
					return w;
				}

			protected:
				virtual void setWorld(World &wld) final
				{
					//
					w = wld;
				}

			private:
				std::reference_wrapper<World> w;
			};

		private:
			//
		};
		inline World::Inhabitant::~Inhabitant() = default;
		using Inhabitant = World::Inhabitant;
	}
}

#endif
