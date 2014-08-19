#ifndef resplunk_meta_Metadatable_HeaderPlusPlus
#define resplunk_meta_Metadatable_HeaderPlusPlus

#include "resplunk/util/Cloneable.hpp"

#include <type_traits>
#include <typeindex>
#include <map>

namespace resplunk
{
	namespace meta
	{
		template<typename T>
		struct Metadata final
		{
			static_assert(std::is_base_of<util::Cloneable, T>::value, "T must be Cloneable");
			Metadata() = default;
			Metadata &operator=(Metadata const &m)
			: t{m.t? std::move(T::Clone(*m.t)) : nullptr}
			{
			}
			Metadata(Metadata &&) = delete;
			Metadata &operator=(Metadata &&) = delete;

			operator bool() const noexcept
			{
				return t;
			}
			template<typename V, typename... Args>
			void emplace(Args &&... args) noexcept
			{
				static_assert(std::is_same<T, V>::value || (std::is_base_of<T, V>::value && std::has_virtual_destructor<T>::value), "Invalid inheritance");
				static_assert(noexcept(V{std::forward<Args>(args)...}), "Constructor not noexcept");
				return t.reset(new V{std::forward<Args>(args)...});
			}
			operator T &() noexcept
			{
				return *t;
			}
			operator T const &() const noexcept
			{
				return *t;
			}

		private:
			std::unique_ptr<T> t {nullptr};
		};

		struct Metadatable
		: util::CloneImplementor<Metadatable>
		{
			Metadatable() = default;

			template<typename T>
			Metadata<T> &meta() noexcept
			{
				auto it = data.find(typeid(T));
				if(it == data.end())
				{
					data.emplace(typeid(T), Data_t::mapped_type{new Meta<T>{*this}});
				}
				return it->second->get<T>();
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

		protected:
			Metadatable(Metadatable const &m)
			{
				for(meta : m.data)
				{
					data.emplace(meta.first, std::move(MetaBase::Clone(*meta.second)));
				}
			}

		private:
			struct MetaBase
			: util::CloneImplementor<MetaBase>
			{
				template<typename T>
				virtual Metadata<T> &get() noexcept = 0;
				template<typename T>
				virtual Metadata<T> const &get() const noexcept = 0;
			};
			template<typename T>
			struct Meta final
			: MetaBase
			, util::CloneImplementor<Meta<T>>
			{
				Metadata<T> m;

				template<>
				virtual Metadata<T> &get<T>() noexcept override
				{
					return m;
				}
				template<>
				virtual Metadata<T> const &get<T>() const noexcept override
				{
					return m;
				}

			private:
				virtual Meta *clone() const noexcept
				{
					return new Meta{*this};
				}
			};

			using Data_t = std::unordered_map<std::type_index, std::unique_ptr<MetaBase>>;
			Data_t data;
		};
	}
}

#endif
