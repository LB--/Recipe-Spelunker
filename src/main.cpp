#include "resplunk/event/CancellableEvent.hpp"
#include "resplunk/server/Server.hpp"

#include <iostream>

using CancellableEvent = resplunk::event::CancellableEvent;
template<typename... Args>
using EventImplementor = resplunk::event::EventImplementor<Args...>;
using Server = resplunk::server::Server;
using ServerSpecific = resplunk::server::ServerSpecific;
template<typename... Args>
using EventProcessor = resplunk::event::EventProcessor<Args...>;
template<typename... Args>
using EventReactor = resplunk::event::EventReactor<Args...>;

struct TestEvent
: EventImplementor<TestEvent, CancellableEvent>
{
	int x;
	TestEvent(Server &s, int x)
	: ServerSpecific(s)
	, x(x)
	{
	}
};

struct TestListener
: EventProcessor<TestEvent>
, EventReactor<TestEvent>
{
	TestListener(Server &s)
	: ServerSpecific(s)
	{
	}

	virtual void onEvent(TestEvent &e) const override
	{
		std::cout << std::endl << "P x = " << e.x << std::endl;
		if(e.x > 10)
		{
			e.x = 10;
		}
		else if(e.x < 0)
		{
			e.cancelled(true);
		}
	}
	virtual void onEvent(TestEvent const &e) override
	{
		std::cout << "R x = " << e.x << std::endl;
	}
};

struct TestEventA
: EventImplementor<TestEventA, TestEvent>
{
	TestEventA(Server &s)
	: ServerSpecific(s)
	, TestEvent(s, 1)
	{
	}
};
struct TestEventB
: EventImplementor<TestEventB, TestEvent>
{
	TestEventB(Server &s)
	: ServerSpecific(s)
	, TestEvent(s, 2)
	{
	}
};

struct DerivedTestEvent
: EventImplementor<DerivedTestEvent, TestEventA, TestEventB>
{
	DerivedTestEvent(Server &s)
	: ServerSpecific(s)
	, TestEvent(s, 4)
	, TestEventA(s)
	, TestEventB(s)
	{
	}
};

struct TestServer
: Server
{
};

int main(int nargs, char **args)
{
	TestServer s;
	TestListener l {s};
	TestEvent{s, 7}.call();
	TestEvent{s, -1}.call();
	TestEvent{s, 14}.call();
	DerivedTestEvent{s}.call();
}
