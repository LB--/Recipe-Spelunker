#ifndef resplunk_event_Events_HeaderPlusPlus
#define resplunk_event_Events_HeaderPlusPlus

#include "resplunk/util/TemplateImplRepo.hpp"

#include <cstdint>
#include <limits>
#include <type_traits>
#include <functional>
#include <memory>
#include <map>
#include <exception>
#include <stack>
#include <set>
#include <tuple>

namespace resplunk
{
	namespace event
	{
		struct Event;

		struct ListenerPriority final
		{
			using Priority_t = std::intmax_t;

			static constexpr Priority_t FIRST = std::numeric_limits<Priority_t>::min();
			static constexpr Priority_t LAST  = std::numeric_limits<Priority_t>::max();

			constexpr ListenerPriority()
			: priority{}
			{
			}
			constexpr ListenerPriority(Priority_t p)
			: priority{p}
			{
			}
			constexpr ListenerPriority(ListenerPriority const &) = default;
			ListenerPriority &operator=(ListenerPriority const &) = delete;
			constexpr ListenerPriority(ListenerPriority &&) = default;
			ListenerPriority &operator=(ListenerPriority &&) = delete;

			constexpr operator Priority_t() const
			{
				return priority;
			}

			friend constexpr bool operator==(ListenerPriority const &a, ListenerPriority const &b)
			{
				return a.priority == b.priority;
			}
			friend constexpr bool operator<(ListenerPriority const &a, ListenerPriority const &b)
			{
				return a.priority < b.priority;
			}

		private:
			Priority_t const priority;
		};

		template<typename EventT>
		struct EventProcessor
		{
			static_assert(std::is_base_of<Event, EventT>::value, "EventT must derive from Event");
			using E = EventT;
			EventProcessor(ListenerPriority priority = ListenerPriority{})
			{
				listen(priority);
			}
			EventProcessor(EventProcessor const &) = delete;
			EventProcessor &operator=(EventProcessor const &) = delete;
			EventProcessor(EventProcessor &&) = delete;
			EventProcessor &operator=(EventProcessor &&) = delete;
			virtual ~EventProcessor()
			{
				ignore();
			}

		protected:
			virtual void listen(ListenerPriority priority = ListenerPriority{}) final
			{
				return E::listen(*this, priority);
			}
			virtual void ignore() final
			{
				return E::ignore(*this);
			}

		private:
			virtual void onEvent(E &e) const = 0;

			friend typename E::Registrar;
		};

		template<typename EventT>
		struct LambdaEventProcessor
		: EventProcessor<EventT>
		{
			using Lambda_t = std::function<void (EventT &e) /*const*/>;
			LambdaEventProcessor(Lambda_t l, ListenerPriority priority = ListenerPriority{})
			: EventProcessor<EventT>(priority)
			, lambda(l)
			{
			}

		private:
			Lambda_t lambda;
			virtual void onEvent(EventT &e) const override
			{
				return lambda(e);
			}
		};

		template<typename EventT>
		struct EventReactor
		{
			static_assert(std::is_base_of<Event, EventT>::value, "EventT must derive from Event");
			using E = EventT;
			EventReactor(ListenerPriority priority = ListenerPriority{})
			{
				listen(priority);
			}
			EventReactor(EventReactor const &) = delete;
			EventReactor &operator=(EventReactor const &) = delete;
			EventReactor(EventReactor &&) = delete;
			EventReactor &operator=(EventReactor &&) = delete;
			virtual ~EventReactor()
			{
				ignore();
			}

		protected:
			virtual void listen(ListenerPriority priority = ListenerPriority{}) final
			{
				return E::listen(*this, priority);
			}
			virtual void ignore() final
			{
				return E::ignore(*this);
			}

		private:
			virtual void onEvent(E const &e) = 0;

			friend typename E::Registrar;
		};

		template<typename EventT>
		struct LambdaEventReactor
		: EventReactor<EventT>
		{
			using Lambda_t = std::function<void (EventT const &e)>;
			LambdaEventReactor(Lambda_t l, ListenerPriority priority = ListenerPriority{})
			: EventReactor<EventT>(priority)
			, lambda(l)
			{
			}

		private:
			Lambda_t lambda;
			virtual void onEvent(EventT const &e) override
			{
				return lambda(e);
			}
		};

		template<typename EventT>
		struct EventRegistrar final
		{
			static_assert(std::is_base_of<Event, EventT>::value, "EventT must derive from Event");
			using E = EventT;
			using Processor = EventProcessor<EventT>;
			using Reactor = EventReactor<EventT>;
			EventRegistrar() = delete;

			static void listen(Processor const &p, ListenerPriority priority = ListenerPriority{})
			{
				processors().emplace(priority, std::cref(p));
			}
			static void listen(Reactor &r, ListenerPriority priority = ListenerPriority{})
			{
				reactors().emplace(priority, std::ref(r));
			}

			static void ignore(Processor const &p)
			{
				auto &procs = processors();
				for(auto it = procs.begin(); it != procs.end(); )
				{
					if(std::addressof(p) == std::addressof(it->second.get()))
					{
						it = procs.erase(it);
					}
					else ++it;
				}
			}
			static void ignore(Reactor &r)
			{
				auto &reacts = reactors();
				for(auto it = reacts.begin(); it != reacts.end(); )
				{
					if(std::addressof(r) == std::addressof(it->second.get()))
					{
						it = reacts.erase(it);
					}
					else ++it;
				}
			}

			static void process(E &e)
			{
				if(!e.shouldProcess())
				{
					return;
				}
				auto &procs = processors();
				for(auto it = procs.begin(); it != procs.end(); ++it)
				{
#if !defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS
					try
#endif
					{
						it->second.get().onEvent(e);
					}
#if !defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS
					catch(std::exception &e)
					{
						//
					}
					catch(...)
					{
						//
					}
#endif
				}
			}
			static void react(E const &e)
			{
				if(!e.shouldReact())
				{
					return;
				}
				auto &reacts = reactors();
				for(auto it = reacts.begin(); it != reacts.end(); ++it)
				{
#if !defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS
					try
#endif
					{
						it->second.get().onEvent(e);
					}
#if !defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS
					catch(std::exception &e)
					{
						//
					}
					catch(...)
					{
						//
					}
#endif
				}
			}

		private:
			struct Key final
			{
				Key() = delete;
			};
			using Processors_t = std::multimap<ListenerPriority, std::reference_wrapper<Processor const>>;
			using Reactors_t = std::multimap<ListenerPriority, std::reference_wrapper<Reactor>>;
			static Processors_t &processors()
			{
				return util::TemplateImplRepo::get<Key, Processors_t>();
			}
			static auto reactors()
			-> Reactors_t &
			{
				return util::TemplateImplRepo::get<Key, Reactors_t>();
			}
		};

		namespace impl
		{
			template<typename T, typename...>
			struct Unwrapper;
			template<typename T, typename First, typename... Rest>
			struct Unwrapper<T, First, Rest...> final
			{
				static_assert(std::is_base_of<Event, First>::value, "ParentT must derive from Event");
				using Next = Unwrapper<T, Rest...>;
				Unwrapper() = delete;
				static void process(T &t)
				{
					t.First::process();
					Next::process(t);
				}
				static void react(T const &t)
				{
					t.First::react();
					Next::react(t);
				}
			};
			template<typename T>
			struct Unwrapper<T> final
			{
				Unwrapper() = delete;
				static void process(T &t)
				{
				}
				static void react(T const &t)
				{
				}
			};
			using Guard_t = std::stack<std::set<std::type_index>>;
			struct ProcessKey final
			{
				ProcessKey() = delete;
			};
			struct ReactKey final
			{
				ReactKey() = delete;
			};
		}
		template<typename EventT, typename... ParentT>
		struct EventImplementor
		: virtual ParentT...
		{
			using Unwrapper = impl::Unwrapper<EventImplementor, ParentT...>;
			using E = EventT;
			using ParentE = std::tuple<ParentT...>;
			using Implementor = EventImplementor;
			using Processor = EventProcessor<EventT>;
			using Reactor = EventReactor<EventT>;
			using Registrar = EventRegistrar<EventT>;
			static constexpr bool MI = (sizeof...(ParentT) > 1);
			EventImplementor() = default;
			virtual ~EventImplementor() = 0;

			static void listen(Processor const &p, ListenerPriority priority = ListenerPriority{})
			{
				return Registrar::listen(p, priority);
			}
			static void listen(Reactor &r, ListenerPriority priority = ListenerPriority{})
			{
				return Registrar::listen(r, priority);
			}

			static void ignore(Processor const &p)
			{
				return Registrar::ignore(p);
			}
			static void ignore(Reactor &r)
			{
				return Registrar::ignore(r);
			}

			virtual void process() override
			{
				using namespace impl;
				Guard_t &guard = util::TemplateImplRepo::get<ProcessKey, Guard_t>();
				if(typeid(*this) == typeid(E))
				{
					guard.emplace();
					guard.top().emplace(typeid(E));
				}
				else if(guard.top().find(typeid(E)) != guard.top().end())
				{
					return;
				}
				else
				{
					guard.top().emplace(typeid(E));
				}

				Unwrapper::process(*this);
				Registrar::process(dynamic_cast<E &>(*this));

				if(typeid(*this) == typeid(E))
				{
					guard.pop();
				}
			}
			virtual void react() const override
			{
				using namespace impl;
				Guard_t &guard = util::TemplateImplRepo::get<ReactKey, Guard_t>();
				if(typeid(*this) == typeid(E))
				{
					guard.emplace();
					guard.top().emplace(typeid(E));
				}
				else if(guard.top().find(typeid(E)) != guard.top().end())
				{
					return;
				}
				else
				{
					guard.top().emplace(typeid(E));
				}

				Unwrapper::react(*this);
				Registrar::react(dynamic_cast<E const &>(*this));

				if(typeid(*this) == typeid(E))
				{
					guard.pop();
				}
			}
		};
		template<typename EventT, typename... ParentT>
		EventImplementor<EventT, ParentT...>::~EventImplementor<EventT, ParentT...>() = default;

		template<typename EventT>
		struct EventImplementor<EventT, void>
		{
			static_assert(std::is_same<EventT, Event>::value, "This can only be derived by Event");
			using E = EventT;
			using ParentE = void;
			using Implementor = EventImplementor;
			using Processor = EventProcessor<EventT>;
			using Reactor = EventReactor<EventT>;
			using Registrar = EventRegistrar<EventT>;
			EventImplementor() = default;
			EventImplementor(EventImplementor const &) = delete;
			EventImplementor &operator=(EventImplementor const &) = delete;
			EventImplementor(EventImplementor &&) = delete;
			EventImplementor &operator=(EventImplementor &&) = delete;
			virtual ~EventImplementor() = 0;

			static void listen(Processor const &p, ListenerPriority priority = ListenerPriority{})
			{
				return Registrar::listen(p, priority);
			}
			static void listen(Reactor &r, ListenerPriority priority = ListenerPriority{})
			{
				return Registrar::listen(r, priority);
			}

			static void ignore(Processor const &p)
			{
				return Registrar::ignore(p);
			}
			static void ignore(Reactor &r)
			{
				return Registrar::ignore(r);
			}

			virtual void process()
			{
				Registrar::process(dynamic_cast<E &>(*this));
			}
			virtual void react() const
			{
				Registrar::react(dynamic_cast<E const &>(*this));
			}
		};
		template<typename EventT>
		EventImplementor<EventT, void>::~EventImplementor<EventT, void>() = default;
	}
}

#endif
