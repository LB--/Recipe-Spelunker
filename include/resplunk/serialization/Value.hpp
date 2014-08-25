#ifndef resplunk_serialization_Value_HeaderPlusPlus
#define resplunk_serialization_Value_HeaderPlusPlus

#include "resplunk/util/Optional.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace resplunk
{
	namespace serialization
	{
		struct Target;
		struct Value final
		{
			enum struct Type
			{
				Null,
				Boolean,
				SignedInt,
				UsignedInt,
				Real,
				String,
				Object,
				List,
				Map,
				Reference
			};
			struct Hint
			{
				Hint() = default;
				Hint(Hint const &) = delete;
				Hint &operator=(Hint const &) = delete;
				Hint(Hint &&) = delete;
				Hint &operator=(Hint &&) = delete;
				virtual ~Hint() = 0;
			};

			Value(std::nullptr_t = nullptr            ) noexcept;
			Value(bool                                ) noexcept;
			Value(std::intmax_t                       ) noexcept;
			Value(std::uintmax_t                      ) noexcept;
			Value(long double                         ) noexcept;
			Value(std::string const &                 ) noexcept;
			Value(Target const &                      ) noexcept;
			Value(std::vector<Value> const &          ) noexcept;
			Value(std::map<std::string, Value> const &) noexcept;
			Value(std::reference_wrapper<Value const> ) noexcept;
			Value(Value const &) noexcept;
			Value &operator=(Value const &) noexcept;
			Value(Value &&) noexcept;
			Value &operator=(Value &&) noexcept;
			~Value() noexcept;

			bool null        (bool exact = true) const noexcept;
			auto boolean     (bool exact = true) const noexcept -> util::Optional<bool>;
			auto signed_int  (bool exact = true) const noexcept -> util::Optional<std::intmax_t>;
			auto unsigned_int(bool exact = true) const noexcept -> util::Optional<std::uintmax_t>;
			auto real        (bool exact = true) const noexcept -> util::Optional<long double>;
			auto string      (bool exact = true) const noexcept -> util::Optional<std::string const &>;
			auto object      (                 ) const noexcept -> util::Optional<Target const &>;
			auto list        (                 ) const noexcept -> util::Optional<std::vector<Value>>;
			auto map         (                 ) const noexcept -> util::Optional<std::map<std::string, Value>>;
			auto reference   (                 ) const noexcept -> util::Optional<std::reference_wrapper<Value const>>;

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;
		};
		inline Value::Hint::~Hint() = default;
	}
}

#endif
