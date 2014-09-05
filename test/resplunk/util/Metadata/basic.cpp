#undef NDEBUG

#include "resplunk/util/Metadata.hpp"

#include <cassert>

using Metadatable = resplunk::util::Metadatable;
template<typename... Args>
using CloneImplementor = resplunk::util::CloneImplementor<Args...>;

int main()
{
	Metadatable m;
	struct TestMeta final
	: CloneImplementor<TestMeta>
	{
		int x = 0;
		TestMeta(Metadatable &m) noexcept
		{
		}

	private:
		TestMeta(TestMeta const &) = default;
		virtual TestMeta *clone() const noexcept override
		{
			return new TestMeta{*this};
		}
	};
	auto &meta = m.meta<TestMeta>();
	if(!meta)
	{
		TestMeta &tm = meta.emplace<TestMeta>(m);
		tm.x = 7;
	}
	{
		TestMeta &tm = meta;
		assert(tm.x == 7);
	}
	{
		Metadatable mc {m};
		TestMeta &tm = mc.meta<TestMeta>();
		assert(tm.x == 7);
	}
}
