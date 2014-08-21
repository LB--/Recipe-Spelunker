#undef NDEBUG

#include "resplunk/util/LiteralString.hpp"

#include <cassert>
#include <string>

using LS = resplunk::util::LiteralString<>;

constexpr LS lsf() noexcept
{
	return "LiteralString";
}

int main() noexcept
{
	assert(std::string(lsf()) == "LiteralString");
	assert(lsf() == lsf());
}
