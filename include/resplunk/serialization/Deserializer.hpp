#ifndef resplunk_serialization_Deserializer_HeaderPlusPlus
#define resplunk_serialization_Deserializer_HeaderPlusPlus

#include "resplunk/serialization/Serializable.hpp"
#include "resplunk/serialization/Value.hpp"

namespace resplunk
{
	namespace serialization
	{
		struct Deserializer
		{
			Deserializer() noexcept = default;
			Deserializer(Deserializer const &) = delete;
			Deserializer &operator=(Deserializer const &) = delete;
			Deserializer(Deserializer &&) = delete;
			Deserializer &operator=(Deserializer &&) = delete;
			virtual ~Deserializer() noexcept = default;

			virtual void deserialize(util::Optional<Serializable> &) const noexcept final
			{
				util::Optional<ObjectValue> object {nullptr};
				if(deserialize(object), object)
				{
					//...
				}
				return nullptr;
			}

			virtual void deserialize(util::Optional<       NullValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<    BooleanValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<  SignedIntValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<UnsignedIntValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<       RealValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<     StringValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<     ObjectValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<       ListValue> &) const noexcept = 0;
			virtual void deserialize(util::Optional<        MapValue> &) const noexcept = 0;
		};
		template<typename ValueT, typename DeserializerT>
		auto Deserialize(DeserializerT &d) noexcept
		-> util::Optional<ValueT>
		{
			util::Optional<ValueT> v;
			d.deserialize(v);
			return v;
		}
	}
}

#endif
