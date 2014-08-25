#include "resplunk/serialization/Target.hpp"

#include <limits>
#include <sstream>

namespace resplunk
{
	namespace serialization
	{
		struct Target::Impl final
		{
			void inherit(Target const &t) noexcept
			{
				//
			}

		private:
			friend Assignable;
			friend Examinable;

			using Values_t = std::map<std::string, Value>;
			using Scopes_t = std::map<std::string, Members_t>;
			Scopes_t scopes;

			auto find(std::string const &scope, std::string const &name) const noexcept
			-> util::Optional<Value const &>
			{
				if(scopes.find(scope) != scopes.end())
				{
					auto values = scopes.at(scope);
					if(values.find(name) != values.end())
					{
						return values.at(name);
					}
				}
				return nullptr;
			}
		};

		Target::Target()
		: impl{new Impl}
		{
		}
		Target::Target(Target const &from)
		: impl{new Impl{*from.impl}}
		{
		}
		Target &operator=(Target const &from)
		{
			*impl = *from.impl;
		}
		Target::Target(Target &&from) noexcept
		: impl(std::move(impl))
		{
		}
		Target &operator=(Target &&from) noexcept
		{
			impl = std::move(from.impl);
		}
		Target::~Target() noexcept
		{
		}

		void Target::inherit(Target const &t) noexcept
		{
			return impl->inherit(t);
		}

		auto Target::Assignable::operator=(std::nullptr_t) noexcept
		-> Target::Assignable &
		{
			auto scopes = target.impl->scopes;
			scopes[scope].erase(name);
			if(scopes[scope].empty())
			{
				scopes.erase(scope);
			}
			return *this;
		}
		auto Target::Assignable::operator=(Value const &v) noexcept
		-> Target::Assignable &
		{
			target.impl->scopes[scope][name] m = v;
			return *this;
		}

		auto Target::Examinable::value() noexcept
		-> util::Optional<Value const &>
		{
			return target.impl->find(scope, name);
		}
	}
}
