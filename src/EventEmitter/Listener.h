#include <Arduino.h>

#ifndef _LISTENER_H_
#define _LISTENER_H_

//#define MAX_EVENT_STRING_SIZE 64

using EventID = String;

template <class ...T>
class Listener {
private:
  EventID eventId;
  void (*callback)(T... t);
  bool once;

public:
  Listener();
  Listener(EventID id, void (*cb)(T... t), bool once) : callback(cb), once(once) {
    eventId = id;
  }

  ~Listener() {}

  EventID getEventId() {
    return eventId;
  }

  bool isOnce() {
    return once;
  }

  void emit(T... t) {
    callback(t...);
  }
};

#endif /* _LISTENER_H_ */