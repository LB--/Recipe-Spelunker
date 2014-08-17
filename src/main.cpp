#include "resplunk/event/Cancellable.hpp"
#include "resplunk/event/Cloneable.hpp"

#include <iostream>
#include <string>

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
template<>
TestEvent::Registrar_t TestEvent::Implementor_t::registrar {};

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

struct TestEvent0 : EventImplementor<TestEvent0, Event> {}; template<> TestEvent0::Registrar_t TestEvent0::Implementor_t::registrar {};

struct TestEvent1 : EventImplementor<TestEvent1, TestEvent0> {}; template<> TestEvent1::Registrar_t TestEvent1::Implementor_t::registrar {};
struct TestEvent2 : EventImplementor<TestEvent2, TestEvent0> {}; template<> TestEvent2::Registrar_t TestEvent2::Implementor_t::registrar {};
struct TestEvent3 : EventImplementor<TestEvent3, TestEvent0> {}; template<> TestEvent3::Registrar_t TestEvent3::Implementor_t::registrar {};
struct TestEvent4 : EventImplementor<TestEvent4, TestEvent0> {}; template<> TestEvent4::Registrar_t TestEvent4::Implementor_t::registrar {};

struct TestEvent5 : EventImplementor<TestEvent5, TestEvent1, TestEvent2> {}; template<> TestEvent5::Registrar_t TestEvent5::Implementor_t::registrar {};
struct TestEvent6 : EventImplementor<TestEvent6, TestEvent3, TestEvent4> {}; template<> TestEvent6::Registrar_t TestEvent6::Implementor_t::registrar {};

struct TestEvent7 : EventImplementor<TestEvent7, TestEvent5, TestEvent6> {}; template<> TestEvent7::Registrar_t TestEvent7::Implementor_t::registrar {};

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
template<>
CloneableTestEvent::Registrar_t CloneableTestEvent::Implementor_t::registrar {};

template<typename T>
auto lep(std::string const &msg)
-> LambdaEventProcessor<T>
{
	return {[=](T &e){ std::cout << msg << std::endl; }};
}

int main(int nargs, char **args) noexcept
{
	LambdaEventProcessor<Event> lepe
	{
		[](Event &e)
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
	}
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
	}
	{
		CloneableTestEvent::Clone(CloneableTestEvent{9})->call();
	}
}
