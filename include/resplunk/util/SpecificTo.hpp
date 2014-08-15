#ifndef resplunk_util_SpecificTo_HeaderPlusPlus
#define resplunk_util_SpecificTo_HeaderPlusPlus

#include <functional>

namespace resplunk
{
	namespace util
	{
		template<typename T, bool LogicalConst = false>
		struct SpecificTo
		{
			using Specific_t = T;
			static constexpr bool LogicalConst_v = LogicalConst;
			using SpecificTo_t = SpecificTo;
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
		template<typename T, bool LC>
		SpecificTo<T, LC>::~SpecificTo<T, LC>() = default;
		template<typename T>
		struct SpecificTo<T, true>
		{
			using Specific_t = T;
			static constexpr bool LogicalConst_v = true;
			using SpecificTo_t = SpecificTo;
			SpecificTo() = delete;
			SpecificTo(SpecificTo const &) = default;
			SpecificTo &operator=(SpecificTo const &) = delete;
			SpecificTo(SpecificTo &&) = default;
			SpecificTo &operator=(SpecificTo &&) = delete;
			virtual ~SpecificTo() = 0;

			virtual Specific_t &specific() const noexcept final
			{
				return spec;
			}
			virtual Specific_t const &specific() noexcept final
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
			mutable std::reference_wrapper<Specific_t> spec;
		};
		template<typename T>
		SpecificTo<T, true>::~SpecificTo<T, true>() = default;
	}
}

#endif
