#include "resplunk/event/CancellableEvent.hpp"
#include "resplunk/server/Server.hpp"

#include <iostream>

struct TestEvent
: resplunk::event::EventImplementor<TestEvent, resplunk::event::CancellableEvent>
{
	int x;
	TestEvent(resplunk::server::Server &s, int x)
	: resplunk::server::ServerSpecific(s)
	, x(x)
	{
	}
};

struct TestListener
: resplunk::event::EventProcessor<TestEvent>
, resplunk::event::EventReactor<TestEvent>
{
	TestListener(resplunk::server::Server &s)
	: resplunk::server::ServerSpecific(s)
	{
	}

	virtual void onEvent(TestEvent &e) const override
	{
		if(e.x > 10)
		{
			e.x = 10;
		}
		else if(e.x < 0)
		{
			e.setCancelled(true);
		}
	}
	virtual void onEvent(TestEvent const &e) override
	{
		std::cout << "x = " << e.x << std::endl;
	}
};

struct DerivedTestEvent
: resplunk::event::EventImplementor<DerivedTestEvent, TestEvent>
{
	DerivedTestEvent(resplunk::server::Server &s)
	: resplunk::server::ServerSpecific(s)
	, TestEvent(s, 4)
	{
	}
};

struct TestServer
: resplunk::server::Server
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
