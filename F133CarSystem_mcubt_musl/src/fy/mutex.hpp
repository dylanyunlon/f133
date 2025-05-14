#ifndef _FY_MUTEX_H_
#define _FY_MUTEX_H_

#include <pthread.h>

namespace fy {

class mutex {
public:
  mutex() {
    pthread_mutex_init(&mutex_, NULL);
  }
  ~mutex() {
    pthread_mutex_destroy(&mutex_);
  }

  void lock() {
    pthread_mutex_lock(&mutex_);
  }
  void unlock() {
    pthread_mutex_unlock(&mutex_);
  }
  int trylock() {
    return pthread_mutex_trylock(&mutex_);
  }

private:
  pthread_mutex_t mutex_;
};

//mutex::mutex() {pthread_mutex_init(&mutex_, NULL);}
//mutex::~mutex() {pthread_mutex_destroy(&mutex_);}
//void mutex::lock() {pthread_mutex_lock(&mutex_);}
//void mutex::unlock() {pthread_mutex_unlock(&mutex_);}
//int mutex::trylock() {return pthread_mutex_trylock(&mutex_);};

template <typename T>
class lock_guard {
public:
  lock_guard(T& mutex):mutex_(mutex) {mutex_.lock();}
  ~lock_guard() {mutex_.unlock();}
private:
  T& mutex_;
};

} /* namespace fy */

#endif
