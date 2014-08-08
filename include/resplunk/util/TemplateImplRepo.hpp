#ifndef ResplunkUtilTemplateImlementationRepository_HeaderPlusPlus
#define ResplunkUtilTemplateImlementationRepository_HeaderPlusPlus

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
					return it->second->r;
				}
				return impls.emplace(t, new Impl).first->second->r;
			}
		};
		inline TemplateImplRepo::ImplBase::~ImplBase() = default;
	};
};

#endif
