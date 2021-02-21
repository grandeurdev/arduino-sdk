#ifndef _LISTENER_H_
#define _LISTENER_H_

template <typename Emitter>
class Listener {
private:
  bool _once;

public:
  Emitter emit;

  Listener();
  /** Constructor: Defines Emitter and whether a listener is of reusable or not.
  */
  Listener(Emitter cb, bool once) : _once(once) {
    emit = cb;
  }

  ~Listener() {}

  /** Checks if a listener is for one-time use
  */
  bool isOnce() {
    return _once;
  }
};

#endif /* _LISTENER_H_ */