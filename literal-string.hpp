#ifndef resplunk_util_LiteralString_HeaderPlusPlus
#define resplunk_util_LiteralString_HeaderPlusPlus

#include <type_traits>
#include <string>

namespace resplunk
{
	namespace util
	{
		template<typename CharT = char>
		struct LiteralString final
		{
			static_assert(std::is_literal_type<LiteralString>::value, "LiteralString is not literal!");
			using Char_t = CharT;
			template<std::size_t N>
			constexpr LiteralString(Char_t const (&str)[N]) noexcept
			: p(str)
			, s(N-1)
			{
			}
			constexpr ~LiteralString() noexcept
			{
			}

			constexpr std::size_t size() const noexcept
			{
				return s;
			}
			constexpr Char_t operator[](std::size_t n) const noexcept
			{
				return n < s? p[n] : Char_t{};
			}
			constexpr Char_t const *begin() const noexcept
			{
				return p;
			}
			constexpr Char_t const *end() const noexcept
			{
				return p+s;
			}
			constexpr Char_t const *cbegin() const noexcept
			{
				return p;
			}
			constexpr Char_t const *cend() const noexcept
			{
				return p+s;
			}

			operator std::basic_string<Char_t>() const noexcept
			{
				return {p, s};
			}

		private:
			Char_t const *p;
			std::size_t const s;
		};
	}
}

#endif
