#ifndef resplunk_util_Metadatable_HeaderPlusPlus
#define resplunk_util_Metadatable_HeaderPlusPlus

#include "resplunk/util/Cloneable.hpp"
#include "resplunk/serialization/Serializable.hpp"

#include <type_traits>
#include <utility>
#include <memory>
#include <typeindex>
#include <map>

namespace resplunk
{
	namespace util
	{
		template<typename T>
		struct Metadata final
		: serialization::Serializable
		{
			static_assert(std::is_base_of<util::Cloneable, T>::value, "T must be Cloneable");
			static_assert(std::is_base_of<serialization::Serializable, T>::value, "T must be Serializable");
			Metadata() noexcept = default;
			Metadata(Metadata const &m) noexcept
			: t{m.t? std::move(T::Clone(*m.t)) : nullptr}
			{
			}
			Metadata &operator=(Metadata m) & noexcept
			{
				swap(m);
				return *this;
			}
			Metadata(Metadata &&) = default;
			Metadata &operator=(Metadata &&) & = default;

			operator bool() const noexcept
			{
				return static_cast<bool>(t);
			}
			template<typename V = T, typename... Args>
			auto emplace(Args &&... args) & noexcept
			-> V &
			{
				static_assert(std::is_same<T, V>::value || (std::is_base_of<T, V>::value && std::has_virtual_destructor<T>::value), "Invalid inheritance");
				static_assert(std::is_nothrow_constructible<V, Args...>::value, "Constructor not noexcept");
				V *vp = new V{std::forward<Args>(args)...};
				t.reset(vp);
				return *vp;
			}
			operator T &() & noexcept
			{
				return *t;
			}
			operator T const &() const noexcept
			{
				return *t;
			}
			auto steal() & noexcept
			-> std::unique_ptr<T>
			{
				return std::move(t);
			}

			struct SerializeEvent final
			: event::Implementor<SerializeEvent, Serializable::SerializeEvent>
			{
				virtual Metadata const &instance() noexcept override
				{
					return inst;
				}

			private:
				Metadata const &inst;
				friend Metadata;
				SerializeEvent(Metadata const &inst, serialization::Target &target)
				: Serializable::SerializeEvent(target)
				, inst(inst)
				{
				}
			};

		private:
			std::unique_ptr<T> t {nullptr};

			virtual SerializeEvent *new_SerializeEvent(serialization::Target &target) const noexcept override
			{
				return new SerializeEvent{*this, target};
			}
		};

		struct Metadatable
		{
			Metadatable() = default;
			Metadatable(Metadatable const &m) noexcept
			{
				for(meta : m.data)
				{
					data.emplace(meta.first, std::move(MetaBase::Clone(*meta.second)));
				}
			}
			Metadatable &operator=(Metadatable m) noexcept
			{
				swap(m);
				return *this;
			}
			Metadatable(Metadatable &&) = default;
			Metadatable &operator=(Metadatable &&) = default;
			virtual ~Metadatable() = default;

			template<typename T>
			Metadata<T> &meta() noexcept
			{
				if(data.find(typeid(T)) == data.end())
				{
					data.emplace(typeid(T), Data_t::mapped_type{new Meta<T>});
				}
				return data.at(typeid(T))->get<T>();
			}
			template<typename T>
			Metadata<T> meta() const noexcept
			{
				auto it = data.find(typeid(T));
				if(it == data.end())
				{
					return {};
				}
				return it->second->get<T>();
			}

			template<typename T>
			void remove() noexcept
			{
				data.erase(typeid(T));
			}

		private:
			struct MetaBase
			: util::CloneImplementor<MetaBase>
			{
				template<typename T>
				auto get() noexcept
				-> Metadata<T> &
				{
					return dynamic_cast<Meta<T> &>(*this).m;
				}
				template<typename T>
				auto get() const noexcept
				-> Metadata<T> const &
				{
					return dynamic_cast<Meta<T> const &>(*this).m;
				}
			};
			template<typename T>
			struct Meta final
			: MetaBase
			, util::CloneImplementor<Meta<T>>
			{
				Metadata<T> m;
				Meta() = default;

			private:
				Meta(Meta const &) = default;
				virtual Meta *clone() const noexcept
				{
					return new Meta{*this};
				}
			};

			using Data_t = std::/*unordered_*/map<std::type_index, std::unique_ptr<MetaBase>>;
			Data_t data;
		};
	}
}

#endif
