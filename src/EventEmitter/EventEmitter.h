#include <map>
#include "Listener.h"

#ifndef _EVENT_EMITTER_H_
#define _EVENT_EMITTER_H_

template<typename EventName, typename Emitter>
class EventEmitter {
private:
  // We use multimap to implement our event emitter.
  using ListenersMap = std::multimap<EventName, Listener<Emitter>*>;
  // We use a vector to store event names uniquely.
  std::vector<EventName> events;

  ListenersMap listeners;

public:
  EventEmitter() {}

  virtual ~EventEmitter() {}
  
  EventName* eventNames() {
    // Returning the array pointer from the vector.
    return events.data();
  }

  size_t getNListeners() {
    // Getting the size of the listeners map.
    return listeners.size();
  }

  void on(EventName eventName, Emitter emitter) {
    // Adding a new reusable listener to listeners map with event name.
    listeners.insert(std::pair<EventName, Listener<Emitter>*>(eventName, new Listener<Emitter>(emitter, false)));
    events.push_back(eventName);
  }

  bool once(EventName eventName, Emitter emitter) {
    // Adding a new nonreusable listener to listeners map with event name.
    listeners.insert(std::pair<EventName, Listener<Emitter>*>(eventName, new Listener<Emitter>(emitter, true)));
    events.push_back(eventName);
  }

  void off(EventName eventName) {
    // Erasing listeners with event name from the listeners map and returning the number of them erased.
    listeners.erase(eventName);
    events.erase(std::find(events.begin(), events.end(), eventName));
  }

  void offAll() {
    // Clearing the listeners map.
    listeners.clear();
    events.clear();
  }

  template<typename ...T>
  void emit(EventName eventName, T... args) {
    // Getting all listeners from listeners map with event name and emit them.
    auto lower_itr = listeners.lower_bound(eventName);
    auto upper_itr = listeners.upper_bound(eventName);
    while (lower_itr != upper_itr) { 
      if (lower_itr->first == eventName) {
        lower_itr->second->emit(args...);
        // If the listener is for once, remove it.
        if(lower_itr->second->isOnce())
          off(eventName);
      }
      lower_itr++;
    }
  }

  template<typename ...T>
  void pEmit(EventName prefix, T... args) {
    // Getting all listeners from listeners map with event name and emit them.
    auto iterator = listeners.lower_bound(prefix);
    if (iterator != listeners.end()) {
        if (String(iterator->first).startsWith(prefix))
          iterator->second->emit(args...);
          // If the listener is for once, remove it.
          if(iterator->second->isOnce())
            off(iterator->first);
    }
  }
};

#endif /* _EVENT_EMITTER_H_ */