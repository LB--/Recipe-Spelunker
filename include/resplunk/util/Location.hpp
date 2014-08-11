#ifndef resplunk_util_Location_HeaderPlusPlus
#define resplunk_util_Location_HeaderPlusPlus

#include <initializer_list>

namespace resplunk
{
	namespace util
	{
		template<typename T>
		struct Location final
		{
			T x = T{}, y = T{}, z = T{};
			Location() = default;
			Location(T x, T y, T z)
			: x{x}
			, y{y}
			, z{z}
			{
			}
			Location(std::initializer_list<T> const &from)
			: x{from.size() >= 1? *(from.begin()+0) : T{}}
			, y{from.size() >= 2? *(from.begin()+1) : T{}}
			, z{from.size() >= 3? *(from.begin()+2) : T{}}
			{
			}
			Location(Location const &) = default;
			Location &operator=(Location const &) = default;
			Location(Location &&) = default;
			Location &operator=(Location &&) = default;
			~Location() = default;

			Location add(T xoff, T yoff, T zoff) const
			{
				return {x + xoff, y + yoff, z + zoff};
			}
		};
	}
}

#endif
