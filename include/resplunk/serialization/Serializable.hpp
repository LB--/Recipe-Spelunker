#ifndef resplunk_serialization_Serializable_HeaderPlusPlus
#define resplunk_serialization_Serializable_HeaderPlusPlus

#include "resplunk/serialization/Value.hpp"
#include "resplunk/event/Cancellable.hpp"

#include <string>
#include <memory>
#include <type_traits>
#include <map>
#include <typeinfo>
#include <typeindex>

namespace resplunk
{
	namespace serialization
	{
		struct Serializable;

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
				static_assert(std::is_base<Serializable, SerializableT>::value);
				auto it = types().emplace(typeid(SeriaizableT), concat(scope...)).first;
				factories().emplace(Factories_t::key_type{it->second}, Factories_t::mapped_type{new FactoryT});
				return it->second;
			}
			template<typename SerializableT>
			static auto Id() noexcept
			-> util::Optional<Id_t const &>
			{
				static_assert(std::is_base<Serializable, SerializableT>::value);
				auto it = types().find(typeid(SerializableT));
				if(it != std::end(types()))
				{
					return it->second;
				}
				return nullptr;
			}
			static auto Implementation(Id_t const &id) noexcept
			-> util::Optional<Factory const &>
			{
				auto it = factories().find(id);
				if(it != std::end(factories()))
				{
					return *(it->second);
				}
				return nullptr;
			}

			std::unique_ptr<Serializable> construct(MapValue const &v) const noexcept;

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

			virtual Serializable *do_construct(MapValue const &) const noexcept = 0;
		};
		template<typename SerializableT>
		struct Factory::Default final
		: virtual Factory
		{
			static_assert(std::is_base<Serializable, SerializableT>::value);

		private:
			virtual SerializableT *do_construct(ObjectValue const &v) const noexcept override
			{
				static_assert(std::is_nothrow_constructible<SerializableT, MapValue const &>::value);
				return new SerializableT{v};
			}
		};

		struct Target final
		{
			using Data_t = std::map<std::reference_wrapper<Factory::Id_t const &>, util::ClonePtr<MapValue>>;
			Data_t data;
			util::Optional<Factory::Id_t const &> type;
		};

		struct Serializable
		{
			virtual ~Serializable() = default;

			struct SerializeEvent
			: event::Implementor<SerializeEvent, event::Cancellable>
			{
				virtual Serializable const &instance() noexcept = 0;

				template<typename SerializableT>
				auto data_for(SerializableT const &s) noexcept
				-> MapValue &
				{
					using SerializeEventT = typename SerializableT::SerializeEvent;
					static_assert(std::is_base_of<SerializeEvent, SerializeEventT>::value);
					Factory::Id_t const &fid = s.factory_id();
					target.type = fid;
					auto it = target.data.find(fid);
					if(it == std::end(target.data))
					{
						return *(target.data.emplace(fid, Data_t::mapped_type{new MapValue}).first->second);
					}
					return *(it->second);
				}

				template<typename SerializableT>
				using type_for = typename SerializableT::SerializeEvent;
				template<typename SerializableT>
				friend auto make_SerializeEvent(SerializableT const &e, Target &t) noexcept
				-> std::unique_ptr<type_for<SerializableT>>
				{
					using EventDerived = type_for<SerializableT>;
					static_assert(std::is_base_of<Serializable, SerializableT>::value);
					static_assert(std::is_base_of<SerializeEvent, EventDerived>::value);
					if(auto p = dynamic_cast<EventDerived *>(e.Serializable::new_SerializeEvent()))
					{
						return std::unique_ptr<EventDerived>{p};
					}
					return std::unique_ptr<EventDerived>{nullptr};
				}

				virtual bool should_react(event::ReactorBase const &) const noexcept override final
				{
					return false;
				}

			protected:
				SerializeEvent(Target &target)
				: target(target)
				{
				}

			private:
				Target &target;
			};

		private:
			virtual Factory::Id_t const &factory_id() const noexcept = 0;

			virtual SerializeEvent *new_SerializeEvent(Target &target) const noexcept = 0;
		};

		auto Factory::construct(MapValue const &v) const noexcept
		-> std::unique_ptr<Serializable>
		{
			return std::move(std::unique_ptr<Serializable>{do_construct(v)});
		}
	}
}

#endif
