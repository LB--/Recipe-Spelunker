#ifndef resplunk_util_TemplateImplRepo_HeaderPlusPlus
#define resplunk_util_TemplateImplRepo_HeaderPlusPlus

#include <cstdint>
#include <typeinfo>
#include <map>
#include <typeindex>
#include <memory>

namespace resplunk
{
	namespace util
	{
		struct TemplateImplRepo final
		{
			TemplateImplRepo() = delete;
			TemplateImplRepo(TemplateImplRepo const &) = delete;
			TemplateImplRepo(TemplateImplRepo &&) = delete;

			template<typename T, typename R, std::uintmax_t I = std::uintmax_t{}>
			static R &get()
			{
				static R r = getImpl<T, R, I>();
				return r;
			}

		private:
			struct ImplBase
			{
				virtual ~ImplBase() = 0;
			};
			using Impls_t = std::map<std::type_index, std::unique_ptr<ImplBase>>;
			static Impls_t impls;

			template<typename T, typename R, std::uintmax_t I>
			static R &getImpl()
			{
				struct Impl final : ImplBase
				{
					R r;
				};
				std::type_index t = typeid(Impl);
				Impls_t::iterator it = impls.find(t);
				if(it != impls.end())
				{
					return dynamic_cast<Impl &>(*it->second.get()).r;
				}
				Impl *impl = new Impl;
				impls.emplace(Impls_t::value_type{t, Impls_t::mapped_type{impl}});
				return impl->r;
			}
		};
		inline TemplateImplRepo::ImplBase::~ImplBase() = default;
	};
};

#endif
