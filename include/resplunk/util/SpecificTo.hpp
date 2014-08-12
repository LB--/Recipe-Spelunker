#ifndef resplunk_util_SpecificTo_HeaderPlusPlus
#define resplunk_util_SpecificTo_HeaderPlusPlus

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

			virtual Specific_t &specific() final
			{
				return spec;
			}
			virtual Specific_t const &specific() const final
			{
				return spec;
			}

		protected:
			SpecificTo(Specific_t &s)
			: spec(s)
			{
			}

		private:
			Specific_t &spec;
		};
		template<typename T>
		inline SpecificTo<T>::~SpecificTo<T>() = default;
	}
}

#endif
