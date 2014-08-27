#ifndef resplunk_util_Optional_HeaderPlusPlus
#define resplunk_util_Optional_HeaderPlusPlus

#include <type_traits>
#include <cstddef>
#include <algorithm>
#include <iterator>
#include <memory>

namespace resplunk
{
	namespace util
	{
		template<typename T>
		struct Optional final
		{
			static_assert(!std::is_polymorphic<T>::value, "T cannot be polymorphic");
			using Value_t = T;
			Optional(std::nullptr_t = nullptr) noexcept
			{
			}
			template<typename... Args>
			Optional(Args &&args) noexcept
			{
				new (&data) T{std::forward<Args>(args)...};
				valid = true;
			}
			Optional(Optional const &from) noexcept
			: Optional(static_cast<T const &>(from))
			{
			}
			Optional &operator=(Optional from) noexcept
			{
				from.valid = false;
				std::swap_ranges(std::begin(data), std::end(data), std::begin(from.data));
				valid = true;
			}
			Optional(Optional &&from) noexcept
			{
				*this = from;
			}
			Optional &operator=(Optional &&from) noexcept
			{
				bool va = valid, vb = from.valid;
				from.valid = false;
				std::swap_ranges(std::begin(data), std::end(data), std::begin(from.data));
				valid = vb;
				from.valid = va;
			}
			~Optional() noexcept
			{
				if(valid)
				{
					valid = false;
					static_cast<T &>(*this).~T();
				}
			}

			operator bool() const noexcept
			{
				return valid;
			}
			operator T &() noexcept
			{
				return *reinterpret_cast<T *>(data);
			}
			operator T const &() const noexcept
			{
				return *reinterpret_cast<T const *>(data);
			}

		private:
			char data[sizeof(T)];
			bool valid = false;
		};
		template<typename T>
		struct Optional<T &> final
		{
			using Value_t = T &;
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
