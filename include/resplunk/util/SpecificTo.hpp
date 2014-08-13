#ifndef resplunk_util_SpecificTo_HeaderPlusPlus
#define resplunk_util_SpecificTo_HeaderPlusPlus

#include <functional>

namespace resplunk
{
	namespace util
	{
		template<typename T>
		struct SpecificTo
		{
			using Specific_t = T;
			SpecificTo() = delete;
			SpecificTo(SpecificTo const &) = default;
			SpecificTo &operator=(SpecificTo const &) = delete;
			SpecificTo(SpecificTo &&) = default;
			SpecificTo &operator=(SpecificTo &&) = delete;
			virtual ~SpecificTo() = 0;

			virtual Specific_t &specific() noexcept final
			{
				return spec;
			}
			virtual Specific_t const &specific() const noexcept final
			{
				return spec;
			}

		protected:
			SpecificTo(Specific_t &s) noexcept
			: spec{s}
			{
			}

			virtual void specific(Specific_t &s) noexcept final
			{
				spec = s;
			}

		private:
			std::reference_wrapper<Specific_t> spec;
		};
		template<typename T>
		inline SpecificTo<T>::~SpecificTo<T>() = default;
	}
}

#endif
