#ifndef resplunk_util_Optional_HeaderPlusPlus
#define resplunk_util_Optional_HeaderPlusPlus

#include <cstddef>
#include <memory>

namespace resplunk
{
	namespace util
	{
		template<typename T>
		struct Optional;
		template<typename T>
		struct Optional<T &> final
		{
			Optional(std::nullptr_t = nullptr) noexcept
			: v(nullptr)
			{
			}
			Optional(T &t) noexcept
			: v(std::addressof(t))
			{
			}
			Optional(Optional const &) = default;
			Optional &operator=(Optional const &) = default;
			Optional(Optional &&) = default;
			Optional &operator=(Optional &&) = default;
			~Optional() = default;

			operator bool() const noexcept
			{
				return v != nullptr;
			}
			operator T &() noexcept
			{
				return *v;
			}
			operator T const &() const noexcept
			{
				return *v;
			}

		private:
			T *v;
		};
	}
}

#endif
