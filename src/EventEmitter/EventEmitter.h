#include "Listener.h"

#ifndef _EVENT_EMITTER_H_
#define _EVENT_EMITTER_H_

#define MAX_LISTENERS 16

template<typename Id, typename Emitter>
class EventEmitter {
private:
  unsigned int nListeners;
  Listener<Id, Emitter> *listeners[MAX_LISTENERS];

public:
  EventEmitter() : nListeners(0) {
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      listeners[i] = NULL;
    }
  }

  virtual ~EventEmitter() {}
  
  Id* getIds() {
    Id ids[nListeners];
    for(unsigned int i = 0; i < nListeners; i++) {
      for (unsigned int j = 0; j < MAX_LISTENERS; j++) {
        if (listeners[j] != NULL) {
          ids[i] = listeners[j]->getId();
        }
      }
    }
    return ids;
  }

  unsigned int getNListeners() {
    return nListeners;
  }

  bool on(Id id, Emitter emitter) {
    if (nListeners < MAX_LISTENERS) {
      for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
        if (listeners[i] == NULL) {
          listeners[i] = new Listener<Id, Emitter>(id, emitter, false);
          nListeners++;
          return true;
        }
      }
    }

    return false;
  }

  bool once(Id id, Emitter emitter) {
    if (nListeners < MAX_LISTENERS) {
      for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
        if (listeners[i] == NULL) {
          listeners[i] = new Listener<Id, Emitter>(id, emitter, true);
          nListeners++;
          return true;
        }
      }
    }

    return false;
  }

  bool off(Id id) {
    bool removed = false;
    // Traverse the listeners array
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      // Get the listener from its id.
      if (listeners[i] != NULL && id == listeners[i]->getId()) {
        delete listeners[i];
        listeners[i] = NULL;
        nListeners--;
        removed = true;
      }
    }

    return removed;
  }

  void offAll() {
    // Traverse the listeners array and remove all listeners.
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      if (listeners[i] != NULL) {
        delete listeners[i];
        listeners[i] = NULL;
        nListeners = 0;
      }
    }
  }

  template<typename ...T> 
  void emit(Id id, T... args) {
    for (unsigned int i = 0; i < MAX_LISTENERS; i++) {
      // Get the listener from its id.
      if (listeners[i] != NULL && id == listeners[i]->getId()) {
        listeners[i]->emit(args...);
        // If the listener is for once, remove it.
        if(listeners[i]->isOnce())
          off(id);
      }
    }
  }

};

#endif /* _EVENT_EMITTER_H_ */