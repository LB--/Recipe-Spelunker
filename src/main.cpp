#include "resplunk/event/CancellableEvent.hpp"
#include "resplunk/server/Server.hpp"

#include <iostream>

struct TestEvent
: resplunk::event::EventImplementor<TestEvent, resplunk::event::CancellableEvent>
{
	int x;
	TestEvent(resplunk::server::Server &s, int x = 7)
	: resplunk::server::ServerSpecific(s)
	, x(x)
	{
	}
};

struct TestListener
: resplunk::event::EventReactor<TestEvent>
{
	TestListener(resplunk::server::Server &s)
		: resplunk::server::ServerSpecific(s)
	, resplunk::event::EventReactor<TestEvent>(0)
	{
	}

	virtual void onEvent(TestEvent const &e) override
	{
		std::cout << "x = " << e.x << std::endl;
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
	TestEvent{s}.call();
}
