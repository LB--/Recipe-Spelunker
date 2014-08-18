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
			using Char_t = CharT;
			static constexpr Char_t EMPTY[] {{}};
			constexpr LiteralString() noexcept
			: LiteralString(EMPTY)
			{
			}
			template<std::size_t N>
			constexpr LiteralString(Char_t const (&str)[N]) noexcept
			: p(str)
			, s(N-1)
			{
			}
			constexpr LiteralString(LiteralString const &from) noexcept
			: p(from.p)
			, s(from.s)
			{
			}
			constexpr LiteralString &operator=(LiteralString const &from) noexcept
			{
				p = from.p;
				s = from.s;
			}
			constexpr LiteralString(LiteralString &&from) noexcept
			: p(std::move(from.p))
			, s(std::move(from.s))
			{
			}
			constexpr LiteralString &operator=(LiteralString &&from) noexcept
			{
				p = std::move(from.p);
				s = std::move(from.s);
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

			friend constexpr bool operator==(LiteralString const &a, LiteralString const &b) noexcept
			{
				if(a.size() == b.size())
				{
					for(std::size_t i = 0; i < a.size(); ++i)
					{
						if(a[i] != b[i])
						{
							return false;
						}
					}
					return true;
				}
				return false;
			}
			friend constexpr bool operator!=(LiteralString const &a, LiteralString const &b) noexcept
			{
				return !(a == b);
			}

			operator std::basic_string<Char_t>() const noexcept
			{
				return {p, s};
			}

		private:
			Char_t const *p;
			std::size_t s;
		};
	}
}

#endif
