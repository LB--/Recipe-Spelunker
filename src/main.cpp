#include "resplunk/event/Cancellable.hpp"
#include "resplunk/event/Cloneable.hpp"
#include "resplunk/util/LiteralString.hpp"
#include "resplunk/meta/Metadata.hpp"

#include <iostream>
#include <string>

template<typename... Args>
using EventImplementor = resplunk::event::Implementor<Args...>;
using Cancellable = resplunk::event::Cancellable;
template<typename... Args>
using EventProcessor = resplunk::event::Processor<Args...>;
template<typename... Args>
using EventReactor = resplunk::event::Reactor<Args...>;
using Cloneable = resplunk::event::Cloneable;
template<typename... Args>
using LambdaEventProcessor = resplunk::event::LambdaProcessor<Args...>;
using Event = resplunk::event::Event;
using ListenerPriority = resplunk::event::ListenerPriority;
using LiteralString = resplunk::util::LiteralString<>;
using Metadatable = resplunk::meta::Metadatable;
template<typename T>
using CloneImplementor = resplunk::util::CloneImplementor<T>;

struct TestEvent
: EventImplementor<TestEvent, Cancellable>
{
	int x;
	TestEvent(int x) noexcept
	: x(x)
	{
	}
};
RESPLUNK_EVENT(TestEvent);

struct TestListener
: EventProcessor<TestEvent>
, EventReactor<TestEvent>
{
private:
	virtual void process(TestEvent &e) const noexcept override
	{
		std::cout << "P x = " << e.x << std::endl;
		if(e.x > 10)
		{
			e.x = 10;
		}
		else if(e.x < 0)
		{
			e.cancelled(true);
		}
	}
	virtual void react(TestEvent const &e) noexcept override
	{
		std::cout << "R x = " << e.x << std::endl;
	}
};

struct TestEvent0 : EventImplementor<TestEvent0, Event> {}; RESPLUNK_EVENT(TestEvent0);

struct TestEvent1 : EventImplementor<TestEvent1, TestEvent0> {}; RESPLUNK_EVENT(TestEvent1);
struct TestEvent2 : EventImplementor<TestEvent2, TestEvent0> {}; RESPLUNK_EVENT(TestEvent2);
struct TestEvent3 : EventImplementor<TestEvent3, TestEvent0> {}; RESPLUNK_EVENT(TestEvent3);
struct TestEvent4 : EventImplementor<TestEvent4, TestEvent0> {}; RESPLUNK_EVENT(TestEvent4);

struct TestEvent5 : EventImplementor<TestEvent5, TestEvent1, TestEvent2> {}; RESPLUNK_EVENT(TestEvent5);
struct TestEvent6 : EventImplementor<TestEvent6, TestEvent3, TestEvent4> {}; RESPLUNK_EVENT(TestEvent6);

struct TestEvent7 : EventImplementor<TestEvent7, TestEvent5, TestEvent6> {}; RESPLUNK_EVENT(TestEvent7);

struct CloneableTestEvent
: EventImplementor<CloneableTestEvent, TestEvent, Cloneable>
{
	CloneableTestEvent(int x) noexcept
	: TestEvent(x)
	{
	}

private:
	virtual CloneableTestEvent *clone() const noexcept override
	{
		return new CloneableTestEvent(x);
	}
};
RESPLUNK_EVENT(CloneableTestEvent);

template<typename T>
auto lep(std::string const &msg)
-> LambdaEventProcessor<T>
{
	return {[=](T &e){ std::cout << msg << " "; }};
}

constexpr LiteralString lsf()
{
	return "LiteralString";
}

int main(int nargs, char **args) noexcept
{
	{
		std::cout << std::string(lsf()) << std::endl;
		std::cout << std::boolalpha << (lsf() == lsf()) << std::endl;
	} std::cout << std::endl;
	LambdaEventProcessor<Event> lepe
	{
		[](Event &e) noexcept
		{
			std::cout << std::endl << "E @ " << &e
			<< " is \"" << typeid(e).name() << '"' << std::endl;
		},
		ListenerPriority::FIRST
	};
	{
		TestListener l;
		TestEvent{7}.call();
		TestEvent{-1}.call();
		TestEvent{14}.call();
	} std::cout << std::endl;
	{
		auto l0 = lep<TestEvent0>("0");
		auto l1 = lep<TestEvent1>("1");
		auto l2 = lep<TestEvent2>("2");
		auto l3 = lep<TestEvent3>("3");
		auto l4 = lep<TestEvent4>("4");
		auto l5 = lep<TestEvent5>("5");
		auto l6 = lep<TestEvent6>("6");
		auto l7 = lep<TestEvent7>("7");
		TestEvent7{}.call();
	} std::cout << std::endl;
	{
		CloneableTestEvent::Clone(CloneableTestEvent{9})->call();
	} std::cout << std::endl;
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
			std::cout << "x = " << tm.x << std::endl;
		}
		{
			auto mc = Metadatable::Clone(m);
			TestMeta &tm = mc->meta<TestMeta>();
			std::cout << "x = " << tm.x << std::endl;
		}
	} std::cout << std::endl;
}
