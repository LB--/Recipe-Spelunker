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
			Metadata(Metadata const &m)
			: t{m.t? std::move(T::Clone(*m.t)) : nullptr}
			{
			}
			Metadata &operator=(Metadata const &m)
			{
				if(m.t)
				{
					t = std::move(T::Clone(*m.t));
				}
				else
				{
					t = nullptr;
				}
				return *this;
			}
			Metadata(Metadata &&) = delete;
			Metadata &operator=(Metadata &&) = delete;

			operator bool() const noexcept
			{
				return static_cast<bool>(t);
			}
			template<typename V, typename... Args>
			void emplace(Args &&... args) noexcept
			{
				static_assert(std::is_same<T, V>::value || (std::is_base_of<T, V>::value && std::has_virtual_destructor<T>::value), "Invalid inheritance");
				static_assert(std::is_nothrow_constructible<V, Args...>::value, "Constructor not noexcept");
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

		protected:
			Metadatable(Metadatable const &m)
			{
				for(meta : m.data)
				{
					data.emplace(meta.first, std::move(MetaBase::Clone(*meta.second)));
				}
			}

		private:
			virtual Metadatable *clone() const noexcept override
			{
				return new Metadatable{*this};
			}

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
				Meta(Meta const &from)
				: m{from.m}
				{
				}
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
