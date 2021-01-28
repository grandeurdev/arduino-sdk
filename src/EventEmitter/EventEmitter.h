#include "Listener.h"

#ifndef _EVENT_EMITTER_H_
#define _EVENT_EMITTER_H_

#define MAX_LISTENERS 16

template <class ...T>
class EventEmitter {
private:
  unsigned int nListeners;
  Listener<T...> *listeners[MAX_LISTENERS];

public:
  EventEmitter() : nListeners(0) {
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      listeners[i] = NULL;
    }
  }

  virtual ~EventEmitter() {}
  
  EventID* getEventIds() {
    EventID eventIds[nListeners];
    for(unsigned int i = 0; i < nListeners; i++) {
      for (unsigned int j = 0; j < MAX_LISTENERS; j++) {
        if (listeners[j] != NULL) {
          eventIds[i] = listeners[j]->getEventId();
        }
      }
    }
    for(int i = 0; i < nListeners; i++)
      Serial.println(eventIds[i]);
    return eventIds;
  }

  unsigned int getNListeners() {
    return nListeners;
  }

  bool on(EventID id, void (*cb)(T... t)) {
    if (nListeners < MAX_LISTENERS) {
      for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
        if (listeners[i] == NULL) {
          listeners[i] = new Listener<T...>(id, cb, false);
          nListeners++;
          return true;
        }
      }
    }

    return false;
  }

  bool once(EventID id, void (*cb)(T... t)) {
    if (nListeners < MAX_LISTENERS) {
      for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
        if (listeners[i] == NULL) {
          listeners[i] = new Listener<T...>(id, cb, true);
          nListeners++;
          return true;
        }
      }
    }

    return false;
  }

  bool off(EventID id) {
    bool removed = false;
    // Traverse the listeners array
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      // Get the listener from its id.
      if (listeners[i] != NULL && id == listeners[i]->getEventId()) {
        delete listeners[i];
        listeners[i] = NULL;
        nListeners--;
        removed = true;
      }
    }

    return removed;
  }

  void removeAllListeners() {
    // Traverse the listeners array and remove all listeners.
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      if (listeners[i] != NULL) {
        delete listeners[i];
        nListeners = 0;
      }
    }
  }

  void emit(EventID id, T... t) {
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      // Get the listener from its id.
      if (listeners[i] != NULL && id == listeners[i]->getEventId()) {
        listeners[i]->emit(t...);
        // If the listener is for once, remove it.
        if(listeners[i]->isOnce())
          off(id);
      }
    }    
  }
};

#endif /* _EVENT_EMITTER_H_ */