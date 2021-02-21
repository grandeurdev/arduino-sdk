#include <map>
#include "Listener.h"

#ifndef _EVENT_EMITTER_H_
#define _EVENT_EMITTER_H_

template<typename EventName, typename Emitter>
class EventEmitter {
private:
  // We use map to implement our event emitter.
  std::multimap<EventName, Listener<Emitter>*> listeners;

public:
  EventEmitter() {}

  virtual ~EventEmitter() {}
  
  EventName* eventNames() {
    // Using a vector to store event names as we do not know the number of keys.
    std::vector<EventName> eventNames;
    // Iterating through the listeners map adding each key to the keys vector.
    for(typename std::multimap<EventName, Listener<Emitter>>::iterator it = listeners.begin(); it != listeners.end(); it = listeners.upper_bound(it->first))
      eventNames.push_back(it->first);
    // Returning the array pointer from the vector.
    return eventNames.data();
  }

  size_t getNListeners() {
    // Getting the size of the listeners map.
    return listeners.size();
  }

  void on(EventName eventName, Emitter emitter) {
    // Adding a new reusable listener to listeners map with event name.
    listeners.insert(std::pair<EventName, Listener<Emitter>*>(eventName, new Listener<Emitter>(emitter, false)));
  }

  bool once(EventName eventName, Emitter emitter) {
    // Adding a new nonreusable listener to listeners map with event name.
    listeners.insert(std::pair<EventName, Listener<Emitter>*>(eventName, new Listener<Emitter>(emitter, true)));
  }

  size_t off(EventName eventName) {
    // Erasing listeners with event name from the listeners map and returning the number of them erased.
    return listeners.erase(eventName);
  }

  void offAll() {
    // Clearing the listeners map.
    listeners.clear();
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