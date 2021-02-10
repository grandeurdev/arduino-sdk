#include <cstring>

#ifndef _LISTENER_H_
#define _LISTENER_H_

//#define MAX_EVENT_STRING_SIZE 64

template <typename Id, typename Emitter>
class Listener {
private:
  Id _id;
  bool _once;

public:
  Emitter emit;

  Listener();
  /** Constructor: Defines Id, Emitter and whether a listener is of one-time use
  */
  Listener(Id id, Emitter cb, bool once) : _once(once) {
    _id = id;
    emit = cb;
  }

  ~Listener() {}

  /** Returns the listener id
  */
  Id getId() {
    return _id;
  }

  /** Checks if a listener is of one-time use
  */
  bool isOnce() {
    return _once;
  }
};

#endif /* _LISTENER_H_ */