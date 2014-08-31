#ifndef resplunk_serialization_Serializable_HeaderPlusPlus
#define resplunk_serialization_Serializable_HeaderPlusPlus

#include "resplunk/util/LiteralString.hpp"
#include "resplunk/serialization/Value.hpp"

#include <memory>
#include <type_traits>
#include <map>
#include <typeinfo>

namespace resplunk
{
	namespace serialization
	{
		struct Serializer;

		struct Factory
		{
			using Registration_t = char const *;
			Factory() noexcept = default;
			Factory(Factory const &) = delete;
			Factory &operator=(Factory const &) = delete;
			Factory(Factory &&) = delete;
			Factory &operator=(Factory &&) = delete;
			virtual ~Factory() noexcept = default;

			template<typename SerializableT>
			struct Default final
			: virtual Factory
			{
				virtual SerializableT *reconstruct(ObjectValue const &v) const noexcept override
				{
					static_assert(std::is_nothrow_constructible<SerializableT, ObjectValue const &>::value);
					return new SerializableT{v};
				}
			};

			template<typename SerializableT, typename FactoryT = Default<SerializableT>>
			static auto register_for() noexcept
			-> Registration_t
			{
				Registration_t name = typeid(Serializable_t).name();
				registrar().emplace(name, Registrar_t::mapped_type{new FactoryT});
				return name;
			}

			template<typename SerializableT>
			static auto Reconstruct(ObjectValue const &v) noexcept
			-> std::unique_ptr<SerializableT>
			{
				Registration_t name = typeid(Serializable_t).name();
				auto it = registrar().find(name);
				if(it != registrar().end())
				{
					return std::move(std::unique_ptr<SerializableT>{it.second->reconstruct(v)});
				}
				return std::move(std::unique_ptr<SerializableT>{nullptr});
			}

		private:
			using Registrar_t = std::map<char const *, std::unique_ptr<Factory>>;
			static auto registrar() noexcept
			-> Registrar_t &
			{
				static Registrar_t r;
				return r;
			}

			virtual Serializable *reconstruct(ObjectValue const &) const noexcept = 0;
		};

		struct Serializable
		{
			virtual ~Serializable() = default;

		protected:
			virtual util::Optional<ObjectValue> serialize() const noexcept = 0;

		private:
			friend Serializer;
			friend ObjectValue;

			virtual Factory::Registration_t factory_id() const noexcept = 0;
		};

		template<typename SerializableT, typename... ParentT>
		struct Implementor
		: virtual Serializable
		, virtual ParentT...
		{
			using Serializable_t = SerializableT;
			using Implementor_t = Implementor;

		private:
			friend SerializeableT;
			Implementor() noexcept = default;

			virtual Factory::Registration_t factory_id() const noexcept override
			{
				return typeid(Serializable_t).name();
			}
		};
	}
}

#endif
