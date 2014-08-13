#include "resplunk/event/Cancellable.hpp"
#include "resplunk/event/Cloneable.hpp"

#include <iostream>

template<typename... Args>
using EventImplementor = resplunk::event::Implementor<Args...>;
using Cancellable = resplunk::event::Cancellable;
template<typename... Args>
using EventProcessor = resplunk::event::Processor<Args...>;
template<typename... Args>
using EventReactor = resplunk::event::Reactor<Args...>;
template<template<typename, typename...> typename Arg = std::unique_ptr, typename... Args>
using Cloneable = resplunk::event::Cloneable<Arg, Args...>;
template<typename... Args>
using LambdaEventProcessor = resplunk::event::LambdaProcessor<Args...>;
using Event = resplunk::event::Event;
using ListenerPriority = resplunk::event::ListenerPriority;

struct TestEvent
: EventImplementor<TestEvent, Cancellable>
{
	int x;
	TestEvent(int x) noexcept
	: x(x)
	{
	}
};

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

struct TestEventA
: EventImplementor<TestEventA, TestEvent>
{
	TestEventA() noexcept
	: TestEvent(1)
	{
	}
};
struct TestEventB
: EventImplementor<TestEventB, TestEvent>
{
	TestEventB() noexcept
	: TestEvent(2)
	{
	}
};

struct DerivedTestEvent
: EventImplementor<DerivedTestEvent, TestEventA, TestEventB>
{
	DerivedTestEvent(int x) noexcept
	: TestEvent(x)
	{
	}
};

struct CloneableTestEvent
: EventImplementor<CloneableTestEvent, TestEvent, Cloneable<>>
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

int main(int nargs, char **args) noexcept
{
	LambdaEventProcessor<Event> lep
	{
		[](Event &e)
		{
			std::cout << std::endl << "E @ " << &e
			<< " is \"" << typeid(e).name() << '"' << std::endl;
		},
		ListenerPriority::FIRST
	};
	TestListener l;
	TestEvent{7}.call();
	TestEvent{-1}.call();
	TestEvent{14}.call();
	DerivedTestEvent{4}.call();
	CloneableTestEvent::Clone(CloneableTestEvent{9})->call();
}
