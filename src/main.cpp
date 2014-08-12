#include "resplunk/event/CancellableEvent.hpp"

#include <iostream>

using CancellableEvent = resplunk::event::CancellableEvent;
template<typename... Args>
using EventImplementor = resplunk::event::EventImplementor<Args...>;
template<typename... Args>
using EventProcessor = resplunk::event::EventProcessor<Args...>;
template<typename... Args>
using EventReactor = resplunk::event::EventReactor<Args...>;
template<typename... Args>
using LambdaEventProcessor = resplunk::event::LambdaEventProcessor<Args...>;
using Event = resplunk::event::Event;
using ListenerPriority = resplunk::event::ListenerPriority;

struct TestEvent
: EventImplementor<TestEvent, CancellableEvent>
{
	int x;
	TestEvent(int x)
	: x(x)
	{
	}
};

struct TestListener
: EventProcessor<TestEvent>
, EventReactor<TestEvent>
{
	TestListener()
	{
	}

	virtual void onEvent(TestEvent &e) const override
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
	virtual void onEvent(TestEvent const &e) override
	{
		std::cout << "R x = " << e.x << std::endl;
	}
};

struct TestEventA
: EventImplementor<TestEventA, TestEvent>
{
	TestEventA()
	: TestEvent(1)
	{
	}
};
struct TestEventB
: EventImplementor<TestEventB, TestEvent>
{
	TestEventB()
	: TestEvent(2)
	{
	}
};

struct DerivedTestEvent
: EventImplementor<DerivedTestEvent, TestEventA, TestEventB>
{
	DerivedTestEvent()
	: TestEvent(4)
	{
	}
};

int main(int nargs, char **args)
{
	LambdaEventProcessor<Event> le
	{
		[](Event &e)
		{
			std::cout << std::endl << "E @ " << &e << std::endl;
		},
		ListenerPriority::FIRST
	};
	TestListener l;
	TestEvent{7}.call();
	TestEvent{-1}.call();
	TestEvent{14}.call();
	DerivedTestEvent{}.call();
}
