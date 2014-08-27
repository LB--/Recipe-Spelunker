#ifndef resplunk_serialization_Value_HeaderPlusPlus
#define resplunk_serialization_Value_HeaderPlusPlus

#include "resplunk/util/Cloneable.hpp"
#include "resplunk/util/Optional.hpp"
#include "resplunk/util/LiteralString.hpp"

#include <type_traits>
#include <cstdint>
#include <string>
#include <limits>
#include <sstream>
#include <map>
#include <vector>
#include <memory>

namespace resplunk
{
	namespace serialization
	{
		struct Value
		: util::CloneImplementor<Value>
		{
			Value() = default;
			Value &operator=(Value const &) = delete;
			Value(Value &&) = delete;
			Value &operator=(Value &&) = delete;
			virtual ~Value() noexcept = default;

			struct Hint
			: util::CloneImplementor<Hint>
			{
				Hint() = default;
				Hint &operator=(Hint const &) = delete;
				Hint(Hint &&) = delete;
				Hint &operator=(Hint &&) = delete;
				virtual ~Hint() noexcept = 0;

			protected:
				Hint(Hint const &) = default;
			};

			auto hint() const noexcept
			-> util::Optional<Hint const &>
			{
				if(h)
				{
					return *h;
				}
				return nullptr;
			}
			template<typename HintT, typename... Args>
			auto hint(Args &&... args) noexcept
			-> HintT &
			{
				static_assert(std::is_base_of<Hint, HintT>::value);
				static_assert(std::is_nothrow_constructible<HintT, Args...>::value);
				HintT *p = new HintT{std::forward<Args>(args)...};
				h.reset(p);
				return *p;
			}
			auto hint(std::nullptr_t) noexcept
			-> std::unique_ptr<Hint>
			{
				return std::move(h);
			}

			virtual bool null() const noexcept
			{
				return false;
			}
			virtual util::Optional<bool> boolean() const noexcept
			{
				return nullptr;
			}
			virtual util::Optional<std::intmax_t> signed_int() const noexcept
			{
				return nullptr;
			}
			virtual util::Optional<std::uintmax_t> unsigned_int() const noexcept
			{
				return nullptr;
			}
			virtual util::Optional<long double> real() const noexcept
			{
				return nullptr;
			}
			virtual util::Optional<std::string> string() const noexcept
			{
				return nullptr;
			}

			template<typename ValueT>
			auto as() const noexcept
			-> util::Optional<ValueT const &>
			{
				static_assert(std::is_base_of<Value, ValueT>::value);
				if(auto p = dynamic_cast<ValueT const *>(this))
				{
					return *p;
				}
				return nullptr;
			}

		protected:
			Value(Value const &) = default;

		private:
			util::ClonePtr<Hint> h {nullptr};
		};
		inline Value::Hint::~Hint() noexcept = default;

		struct NullValue final
		: virtual Value
		, util::CloneImplementor<NullValue>
		{
			using Value_t = std::nullptr_t;
			NullValue(Value_t = nullptr) noexcept
			{
			}

			operator Value_t() const noexcept
			{
				return nullptr;
			}

			virtual bool null() const noexcept override
			{
				return true;
			}

		protected:
			NullValue(NullValue const &) = default;

		private:
			virtual NullValue *clone() const noexcept override
			{
				return new NullValue{*this};
			}
		};

		struct BooleanValue final
		: virtual Value
		, util::CloneImplementor<BooleanValue>
		{
			using Value_t = bool;
			BooleanValue(Value_t v = Value_t{})
			: v(v)
			{
			}

			operator Value_t() const noexcept
			{
				return v;
			}

			virtual util::Optional<bool> boolean() const noexcept override
			{
				return v;
			}
			virtual util::Optional<std::intmax_t> signed_int() const noexcept override
			{
				return v? 1 : 0;
			}
			virtual util::Optional<std::uintmax_t> unsigned_int() const noexcept override
			{
				return v? 1 : 0;
			}
			virtual util::Optional<std::string> string() const noexcept override
			{
				return v? "true" : "false";
			}

		protected:
			BooleanValue(BooleanValue const &) = default;

		private:
			Value_t v;

			virtual BooleanValue *clone() const noexcept override
			{
				return new BooleanValue{*this};
			}
		};

		struct SignedIntValue final
		: virtual Value
		, util::CloneImplementor<SignedIntValue>
		{
			using Value_t = std::intmax_t;
			SignedIntValue(Value_t v = Value_t{})
			: v(v)
			{
			}

			operator Value_t() const noexcept
			{
				return v;
			}

			virtual util::Optional<bool> boolean() const noexcept override
			{
				if(v == 0)
				{
					return false;
				}
				else if(v == 1)
				{
					return true;
				}
				return nullptr;
			}
			virtual util::Optional<std::intmax_t> signed_int() const noexcept override
			{
				return v;
			}
			virtual util::Optional<std::uintmax_t> unsigned_int() const noexcept override
			{
				if(v >= 0)
				{
					return static_cast<std::uintmax_t>(v);
				}
				return nullptr;
			}
			virtual util::Optional<long double> real() const noexcept override
			{
				return static_cast<long double>(v);
			}
			virtual util::Optional<std::string> string() const noexcept override
			{
				std::ostringstream oss;
				if(oss << v)
				{
					return oss.str();
				}
				return nullptr;
			}

		protected:
			SignedIntValue(SignedIntValue const &) = default;

		private:
			Value_t v;

			SignedIntValue *clone() const noexcept override
			{
				return new SignedIntValue{*this};
			}
		};

		struct UnsignedIntValue final
		: virtual Value
		, util::CloneImplementor<UnsignedIntValue>
		{
			using Value_t = std::uintmax_t;
			UnsignedIntValue(Value_t v = Value_t{})
			: v(v)
			{
			}

			operator Value_t() const noexcept
			{
				return v;
			}

			virtual util::Optional<bool> boolean() const noexcept override
			{
				if(v == 0u)
				{
					return false;
				}
				else if(v == 1u)
				{
					return true;
				}
				return nullptr;
			}
			virtual util::Optional<std::intmax_t> signed_int() const noexcept override
			{
				if(v <= std::numeric_limits<std::intmax_t>::max())
				{
					return static_cast<std::intmax_t>(v);
				}
				return nullptr;
			}
			virtual util::Optional<std::uintmax_t> unsigned_int() const noexcept override
			{
				return v;
			}
			virtual util::Optional<long double> real() const noexcept override
			{
				return static_cast<long double>(v);
			}
			virtual util::Optional<std::string> string() const noexcept override
			{
				std::ostringstream oss;
				if(oss << v)
				{
					return oss.str();
				}
				return nullptr;
			}

		protected:
			UnsignedIntValue(UnsignedIntValue const &) = default;

		private:
			Value_t v;

			UnsignedIntValue *clone() const noexcept override
			{
				return new UnsignedIntValue{*this};
			}
		};

		struct RealValue final
		: virtual Value
		, util::CloneImplementor<RealValue>
		{
			using Value_t = long double;
			RealValue(Value_t v = Value_t{})
			: v(v)
			{
			}

			operator Value_t() const noexcept
			{
				return v;
			}

			virtual util::Optional<std::intmax_t> signed_int() const noexcept override
			{
				if(v >= std::numeric_limits<std::intmax_t>::min()
				&& v <= std::numeric_limits<std::intmax_t>::max())
				{
					return static_cast<std::intmax_t>(v);
				}
				return nullptr;
			}
			virtual util::Optional<std::uintmax_t> unsigned_int() const noexcept override
			{
				if(v >= 0
				&& v <= std::numeric_limits<std::uintmax_t>::max())
				{
					return static_cast<std::uintmax_t>(v);
				}
				return nullptr;
			}
			virtual util::Optional<long double> real() const noexcept override
			{
				return v;
			}
			virtual util::Optional<std::string> string() const noexcept override
			{
				std::ostringstream oss;
				if(oss << v)
				{
					return oss.str();
				}
				return nullptr;
			}

		protected:
			RealValue(RealValue const &) = default;

		private:
			Value_t v;

			virtual RealValue *clone() const noexcept override
			{
				return new RealValue{*this};
			}
		};

		struct StringValue final
		: virtual Value
		, util::CloneImplementor<StringValue>
		{
			using Value_t = std::string;
			StringValue(Value_t const &v = Value_t{})
			: v(v)
			{
			}

			operator Value_t const &() const noexcept
			{
				return v;
			}

			virtual util::Optional<bool> boolean() const noexcept override
			{
				if(v == "true")
				{
					return true;
				}
				else if(v == "false")
				{
					return false;
				}
				return nullptr;
			}
			virtual util::Optional<std::intmax_t> signed_int() const noexcept override
			{
				std::istringstream iss;
				std::intmax_t r;
				if(iss >> r)
				{
					return r;
				}
				return nullptr;
			}
			virtual util::Optional<std::uintmax_t> unsigned_int() const noexcept override
			{
				std::istringstream iss;
				std::uintmax_t r;
				if(iss >> r)
				{
					return r;
				}
				return nullptr;
			}
			virtual util::Optional<long double> real() const noexcept override
			{
				std::istringstream iss;
				long double r;
				if(iss >> r)
				{
					return r;
				}
				return nullptr;
			}
			virtual util::Optional<std::string> string() const noexcept override
			{
				return v;
			}

		protected:
			StringValue(StringValue const &) = default;

		private:
			Value_t v;

			virtual StringValue *clone() const noexcept override
			{
				return new StringValue{*this};
			}
		};

		struct ObjectValue final
		: virtual Value
		, util::CloneImplementor<ObjectValue>
		{
			ObjectValue() = default;
			ObjectValue(ObjectValue &&) = default;
			ObjectValue &operator=(ObjectValue &&) = default;

			template<typename SerializableT>
			auto as() noexcept
			{
				static constexpr util::LiteralString const &scope = SerializableT::SCOPE;
				struct Helper final
				{
					ObjectValue &inst;
					auto operator[](std::string const &n) noexcept
					{
						static constexpr std::string const scope_str {scope};
						struct Assignable final
						{
							ObjectValue &inst;
							std::string n;
							Assignable &operator=(std::nullptr_t) noexcept
							{
								inst.values.erase(n);
								return *this;
							}
							Assignable &operator=(Value const &v) noexcept
							{
								inst.values.emplace(n, Value::Clone(v));
								return *this;
							}
						};
						return Assignable{inst, scope_str+'\0'+n};
					}
				};
				return Helper{*this};
			}
			template<typename SerializableT>
			auto as() const noexcept
			{
				static constexpr util::LiteralString const &scope = SerializableT::SCOPE;
				struct Helper final
				{
					ObjectValue &inst;
					auto operator[](std::string const &n) noexcept
					-> util::Optional<Value const &>
					{
						static constexpr std::string const scope_str {scope};
						auto it = inst.values.find(scope_str+'\0'+n);
						if(it != inst.values.end())
						{
							return *(it->second);
						}
						return nullptr;
					}
				};
				return Helper{*this};
			}

			//type information...

		protected:
			ObjectValue(ObjectValue const &) = default;

		private:
			using Values_t = std::map<std::string, util::ClonePtr<Value>>;
			Values_t values;

			virtual ObjectValue *clone() const noexcept override
			{
				return new ObjectValue{*this};
			}
		};

		struct ListValue final
		: virtual Value
		, util::CloneImplementor<ListValue>
		{
			ListValue() = default;

			//

		protected:
			ListValue(ListValue const &) = default;

		private:
			using List_t = std::vector<util::ClonePtr<Value>>;
			List_t list;

			virtual ListValue *clone() const noexcept override
			{
				return new ListValue{*this};
			}
		};

		struct MapValue final
		: virtual Value
		, util::CloneImplementor<MapValue>
		{
			MapValue() = default;

			//

		protected:
			MapValue(MapValue const &) = default;

		private:
			//

			virtual MapValue *clone() const noexcept override
			{
				return new MapValue{*this};
			}
		};
	}
}

#endif
