#ifndef _FY_BOUNDED_BUFFER_H_
#define _FY_BOUNDED_BUFFER_H_

#include <string>
#include <vector>
#include <system/Thread.h>
#include <utils/Log.h>

namespace fy {

template<typename T>
class Boundedbuffer: protected Thread {
public:
  Boundedbuffer(size_t capacity) {
    capacity_ = capacity;
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&cond_, NULL);
  }

  virtual ~Boundedbuffer() {
    pthread_cond_signal(&cond_);
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
  }

  int Deposit(T item) {
    if (!isRunning()) {
      run("boundedbuffer");
    }
    if (v_.size() >= capacity_) {
      return v_.size();
    }
    pthread_mutex_lock(&mutex_);
    v_.push_back(item);
    int size = v_.size();
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
    return size;
  }

  int DepositFront(T item) {
    if (!isRunning()) {
      run("boundedbuffer");
    }
    if (v_.size() >= capacity_) {
      return v_.size();
    }
    pthread_mutex_lock(&mutex_);
    v_.insert(v_.begin(), item);
    int size = v_.size();
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
    return size;
  }

  T Retrieve() {
    while (true) {
      pthread_mutex_lock(&mutex_);
      if (v_.size() > 0) {
        T i = v_.at(0);
        v_.erase(v_.begin());
        pthread_mutex_unlock(&mutex_);
        return i;
      }
      pthread_cond_wait(&cond_, &mutex_);
      pthread_mutex_unlock(&mutex_);
    }
  }

protected:
  virtual bool threadLoop() {
    T item = Retrieve();
    OnRetrieve(item);
    return true;
  }

  virtual void OnRetrieve(T const item) {
  }

private:
  size_t capacity_;
  std::vector<T> v_;
  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
};

} /* namespace fly */

#endif
