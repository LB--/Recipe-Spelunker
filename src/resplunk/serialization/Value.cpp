#include "resplunk/serialization/Value.hpp"
#include "resplunk/serialization/Serializable.hpp"

namespace resplunk
{
	namespace serialization
	{
		struct ObjectValue::Impl final
		{
			Target target;
		};
		ObjectValue::ObjectValue(Serializable const &s) noexcept
		: impl{new Impl}
		{
			make_SerializeEvent(s, impl->target)->call();
		}
		ObjectValue::ObjectValue(ObjectValue const &from) noexcept
		: impl{new Impl{*from.impl}}
		{
		}
		ObjectValue::~ObjectValue() noexcept = default;
		auto ObjectValue::target() noexcept
		-> Target &
		{
			return impl->target;
		}
		auto ObjectValue::target() const noexcept
		-> Target const &
		{
			return impl->target;
		}
	}
}
