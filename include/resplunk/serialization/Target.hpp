#ifndef resplunk_serialization_Target_HeaderPlusPlus
#define resplunk_serialization_Target_HeaderPlusPlus

#include "resplunk/serialization/Serlializable.hpp"
#include "resplunk/serialization/Value.hpp"

#include <cstdint>
#include <string>
#include <functional>
#include <memory>

namespace resplunk
{
	namespace serialization
	{
		struct Target final
		{
			Target() noexcept;
			Target(Target const &) noexcept;
			Target &operator=(Target const &) noexcept;
			Target(Target &&) noexcept;
			Target &operator=(Target &&) noexcept;
			~Target() noexcept;

			void inherit(Target const &) noexcept;

			template<typename SerializableT>
			auto write_as() noexcept
			{
				static constexpr util::LiteralString const &scope = SerializableT::SCOPE;

				struct Helper
				{
					Assignable operator[](std::string const &name) noexcept
					{
						return {target, scope, name};
					}

				private:
					friend Target;
					Target &target;
					Helper(Target &t)
					: target(t)
					{
					}
				};

				return Helper{*this};
			}
			template<typename SerializableT>
			auto read_as() const noexcept
			{
				static constexpr util::LiteralString const &scope = SerializableT::SCOPE;

				struct Helper
				{
					Examinable operator[](std::string const &name) const noexcept
					{
						return {target, scope, name};
					}

				private:
					friend Target;
					Target const &target;
					Helper(Target const &t)
					: target(t)
					{
					}
				};

				return Helper{*this}.value();
			}

		private:
			struct Impl;
			std::unique_ptr<Impl> impl;

			struct Assignable final
			{
				Assignable &operator=(std::nullptr_t) noexcept;
				Assignable &operator=(Value const &v) noexcept;

			private:
				friend Target;
				Target &target;
				util::LiteralString const &scope;
				std::string name;
				Assignable(Target &target, util::LiteralString const &scope, std::string const &name) noexcept
				: target(target)
				, scope(scope)
				, name(name)
				{
				}
			};
			struct Examinable final
			{
				auto value() noexcept -> util::Optional<Value const &>;

			private:
				friend Target;
				Target const &target;
				util::LiteralString const &scope;
				std::string name;
				Examinable(Target const &target, util::LiteralString const &scope, std::string const &name) noexcept
				: target(target)
				, scope(scope)
				, name(name)
				{
				}
			};
		};
	}
}

#endif
