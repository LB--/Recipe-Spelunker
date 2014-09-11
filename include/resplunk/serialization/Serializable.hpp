#ifndef resplunk_serialization_Serializable_HeaderPlusPlus
#define resplunk_serialization_Serializable_HeaderPlusPlus

#include "resplunk/util/LiteralString.hpp"
#include "resplunk/serialization/Value.hpp"

#include <memory>
#include <type_traits>
#include <map>
#include <typeinfo>
#include <typeindex>

namespace resplunk
{
	namespace serialization
	{
		struct Serializer;

		struct Factory
		{
			using Id_t = std::string;
			Factory() noexcept = default;
			Factory(Factory const &) = delete;
			Factory &operator=(Factory const &) = delete;
			Factory(Factory &&) = delete;
			Factory &operator=(Factory &&) = delete;
			virtual ~Factory() noexcept = default;

			template<typename SerializableT>
			struct Default;

			template<typename SerializableT, typename FactoryT = Default<SerializableT>, std::size_t... N>
			static auto Register(char const (&scope)[N]...) noexcept
			-> Id_t const &
			{
				static_assert(sizeof...(N) > 0, "Global scope is reserved");
				static_assert(first_size<N...> > 0, "The empty scope is reserved for implementation details");
				auto it = types().emplace(typeid(SeriaizableT), concat(scope...)).first;
				factories().emplace(Factories_t::key_type{it->second}, Factories_t::mapped_type{new FactoryT});
				return it->second;
			}
			template<typename SerializableT>
			static auto Id() noexcept
			-> util::Optional<Id_t const &>
			{
				auto it = types().find(typeid(SerializableT));
				if(it != types().end())
				{
					return it->second;
				}
				return nullptr;
			}

			template<typename SerializableT>
			static auto Reconstruct(ObjectValue const &v) noexcept
			-> std::unique_ptr<SerializableT>
			{
				Id_t const &name = v.factory_id();
				auto it = factories().find(name);
				if(it != factories().end())
				{
					return std::move(std::unique_ptr<SerializableT>{it.second->reconstruct(v)});
				}
				return std::move(std::unique_ptr<SerializableT>{nullptr});
			}

		private:
			template<std::size_t Last>
			static auto concat(char const (&last)[Last]) noexcept
			{
				return last + std::string{'\0'};
			}
			template<std::size_t First, std::size_t... Rest>
			static auto concat(char const (&first)[First], char const (&rest)[Rest]...) noexcept
			{
				return first + std::string{'\0'} + concat(rest...);
			}
			template<std::size_t First, std::size_t...>
			static constexpr std::size_t first_size {First};

			struct factories_t_compare final
			{
				bool operator<(std::reference_wrapper<Id_t> a, std::reference_wrapper<Id_t> b) const noexcept
				{
					return a.get() < b.get();
				}
			};
			using Factories_t = std::map<std::reference_wrapper<Id_t>, std::unique_ptr<Factory>, factories_t_compare>;
			using Types_t = std::map<std::type_index, Id_t>;
			static auto factories() noexcept
			-> Factories_t &
			{
				static Factories_t r;
				return r;
			}
			static auto types() noexcept
			-> Types_t &
			{
				static Types_t t;
				return t;
			}

			virtual Serializable *reconstruct(ObjectValue const &) const noexcept = 0;
		};
		template<typename SerializableT>
		struct Factory::Default final
		: virtual Factory
		{
			virtual SerializableT *reconstruct(ObjectValue const &v) const noexcept override
			{
				static_assert(std::is_nothrow_constructible<SerializableT, ObjectValue const &>::value);
				return new SerializableT{v};
			}
		};

		struct Serializable
		{
			virtual ~Serializable() = default;

		protected:
			virtual util::Optional<ObjectValue> serialize() const noexcept = 0;

		private:
			friend Serializer;
			friend ObjectValue;

			virtual Factory::Id_t const &factory_id() const noexcept = 0;
		};
	}
}

#endif
