#pragma once
#include <list>
#include <functional>


// template<typename ...functionArgs>
// using Event = EventHandler<functionArgs...>;

template<typename returnType, typename... Args>
class Event {
   bool initiated;
   std::function<returnType(Args...)> member;

public:
   Event() : initiated(false) { }

   Event(std::function<returnType(Args...)> func) : member(func), initiated(true) { }

   returnType operator()(Args ... args) {
      if (initiated) {
         return member(args...);
      }
      return returnType();
   }

   Event<returnType, Args...>& operator=(std::function<returnType(Args...)> func) {
      member = func;
      initiated = true;
   }
};

template<typename... functionArgs>
class EventHandler
{
	std::list<Event<void(functionArgs)> list;

public:
	int subscribe(Event<void(functionArgs)>&& function);

	void unsubscribe(int id);

	void invoke(functionArgs... args);

	void operator bool();
};

