#ifndef resplunk_serialization_Serializer_HeaderPlusPlus
#define resplunk_serialization_Serializer_HeaderPlusPlus

namespace resplunk
{
	namespace serialization
	{
		struct Serializable;

		struct NullValue;
		struct BooleanValue;
		struct SignedIntValue;
		struct UnsignedIntValue;
		struct RealValue;
		struct StringValue;
		struct ObjectValue;
		struct ListValue;
		struct MapValue;

		struct Translator;

		struct Serializer
		{
			Serializer() noexcept = default;
			Serializer(Serializer const &) = delete;
			Serializer &operator=(Serializer const &) = delete;
			Serializer(Serializer &&) = delete;
			Serializer &operator=(Serializer &&) = delete;
			virtual ~Serializer() noexcept = default;

			virtual void serialize(Serializable const &) const noexcept final;

			virtual void serialize(       NullValue const &) const noexcept = 0;
			virtual void serialize(    BooleanValue const &) const noexcept = 0;
			virtual void serialize(  SignedIntValue const &) const noexcept = 0;
			virtual void serialize(UnsignedIntValue const &) const noexcept = 0;
			virtual void serialize(       RealValue const &) const noexcept = 0;
			virtual void serialize(     StringValue const &) const noexcept = 0;
			virtual void serialize(     ObjectValue const &) const noexcept = 0;
			virtual void serialize(       ListValue const &) const noexcept = 0;
			virtual void serialize(        MapValue const &) const noexcept = 0;
		};
	}
}

#endif
