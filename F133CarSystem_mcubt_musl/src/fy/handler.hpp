#ifndef _FY_HANDLER_HPP_
#define _FY_HANDLER_HPP_

#include <unistd.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <list>

#include <system/Thread.h>
#include "time.hpp"
#include "utils/Log.h"


#define TEMP_FAILURE_RETRY2(exp) ({         \
    int _rc;                      \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })


namespace fy {

class handler;

class message {
public:
  message(int what, void* obj = NULL, int arg1 = 0, int arg2 = 0):
	  what(what),obj(obj),arg1(arg1),arg2(arg2),when_(0),period_(0),runable_(NULL) {
  }

  int what;
  void* obj;
  int arg1;
  int arg2;
private:
  int64_t when_;
  int64_t period_;
  void* runable_;
  friend class fy::handler;
};

class handler {
public:
  typedef void (*func_message_handler)(const message* msg, const void* user_data);
  typedef void (*func_runable)(const void* user_data);

public:
  explicit handler(func_message_handler func_handler, const void* user_data)
  :looper_(this), user_data_((void*)user_data) {
    initialize();
    message_handler_ = func_handler;
  }

  explicit handler():looper_(this), user_data_(NULL) {
    initialize();
  }

  ~handler() {
    exit_ = true;
    wake();
    looper_.requestExitAndWait();
    close(epoll_fd_);
    close(wake_event_fd_);
    message_queue_.clear();
  }

  /**
   * typedef void (*func_message_handler)(const message* msg, const void* user_data);
   */
  void set_message_handler(func_message_handler func, const void* user_data) {
    message_handler_ = func;
    user_data_ = (void*)user_data;
  }

  void send_message(int what) {
    message msg(what);
    enqueue_with_lock(msg);
  }

  void send_message(const message &msg) {
    enqueue_with_lock(msg);
  }

  void send_message_delayed(int what, int delay_millis) {
    message msg(what);
    msg.when_ = fy::time::uptime() + delay_millis;
    enqueue_with_lock(msg);
  }

  void send_message_delayed(message &msg, int delay_millis) {
    msg.when_ = fy::time::uptime() + delay_millis;
    enqueue_with_lock(msg);
  }

  void post(func_runable runable, const void* user_data) {
    message msg(0);
    msg.runable_ = (void*)runable;
    msg.obj = (void*)user_data;
    enqueue_with_lock(msg);
  }

  /**
   * typedef void (*func_runable)(const void* user_data);
   */
  void post_delayed(func_runable runable, const void* user_data, int delay_millis) {
    message msg(0);
    msg.runable_ = (void*)runable;
    msg.obj = (void*)user_data;
    msg.when_ = fy::time::uptime() + delay_millis;
    enqueue_with_lock(msg);
  }

  void schedule(int what, int period_millis, int delay_millis) {
    message msg(what);
    msg.when_ = fy::time::uptime() + delay_millis;
    msg.period_ = period_millis;
    enqueue_with_lock(msg);
  }

  void schedule(message &msg, int period_millis, int delay_millis) {
    msg.when_ = fy::time::uptime() + delay_millis;
    msg.period_ = period_millis;
    enqueue_with_lock(msg);
  }

  void remove_messages(int what) {
    Mutex::Autolock lock(mutex_);
    std::list<message>::iterator it;
    for (it = message_queue_.begin(); it != message_queue_.end(); ) {
      if (it->what == what) {
        it = message_queue_.erase(it);
      } else {
        ++it;
      }
    }
  }

  bool has_message(int what) {
    Mutex::Autolock lock(mutex_);
    std::list<message>::iterator it;
    for (it = message_queue_.begin(); it != message_queue_.end(); ) {
      if (it->what == what) {
        return true;
      } else {
        ++it;
      }
    }
    return false;
  }

private:
  void initialize() {
    init_ = false;
    wake_event_fd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    message_handler_ = NULL;
    const int kEpollSizeHint  = 1;
    epoll_fd_ = epoll_create(kEpollSizeHint);
    if (epoll_fd_ < 0) {
      LOGE("%s %d epoll_create failed", __func__, __LINE__);
      return;
    }
    struct epoll_event eventItem;
     memset(&eventItem, 0, sizeof(epoll_event));
     eventItem.events = EPOLLIN;
     eventItem.data.fd = wake_event_fd_;
     int result = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, wake_event_fd_, &eventItem);
     if (result < 0) {
       LOGE("%s %d epoll_ctl failed", __func__, __LINE__);
       return;
     }
    exit_ = false;
    init_ = true;
    looper_.run("sup.handler");
  }

  void loop() {
    while (!exit_) {
      if (message_queue_.empty()) {
        awoken();
        wait(10 * 1000);
        continue;
      }
      mutex_.lock();
      fy::message front = message_queue_.front();
      int64_t now = fy::time::uptime();

      if (front.when_ <= now) {
        message_queue_.pop_front();
        if (front.period_ > 0) {
          front.when_ = fy::time::uptime() + front.period_;
          enqueue(front);
        }
        mutex_.unlock();
        if (front.runable_) {
          ((func_runable)front.runable_)(front.obj);
        } else if (message_handler_) {
          message_handler_(&front, user_data_);
        }
        continue;
      }
      mutex_.unlock();
      awoken();
      wait(front.when_ - now);
    }
  }

  void wait(int timeout_millis) {
    const int kEpollMaxEvents = 1;
    struct epoll_event eventItems[kEpollMaxEvents];
    int event_count = epoll_wait(epoll_fd_, eventItems, kEpollMaxEvents, timeout_millis);
    if (event_count < 0) {
      LOGE("%s %d epoll_wait failed", __func__, __LINE__);
    }
  }

  void wake() {
    uint64_t inc = 1;
    ssize_t nWrite = TEMP_FAILURE_RETRY2(write(wake_event_fd_, &inc, sizeof(uint64_t)));
    if (nWrite != sizeof(uint64_t)) {
        if (errno != EAGAIN) {
            LOGE("Could not write wake signal, errno=%d", errno);
        }
    }
  }

  void awoken() {
    uint64_t counter;
    TEMP_FAILURE_RETRY2(read(wake_event_fd_, &counter, sizeof(uint64_t)));
  }

  void enqueue(const message &msg) {
    std::list<message>::iterator it;
    for (it = message_queue_.begin(); it != message_queue_.end(); ++it) {
      if (msg.when_ < it->when_) {
        it = message_queue_.insert(it, msg);
        if (it == message_queue_.begin()) {
          wake();
        }
        return;
      }
    }
    message_queue_.push_back(msg);
    if (message_queue_.size() == 1) {
      wake();
    }
  }

  void enqueue_with_lock(const message &msg) {
    Mutex::Autolock lock(mutex_);
    enqueue(msg);
  }

  class Looper : public Thread {
  public:
    Looper(handler *handler):handler_(handler){}
    virtual bool threadLoop() {
      handler_->loop();
      return false;
    }
    handler* handler_;
  };

  Looper looper_;
  std::list<message> message_queue_;
  func_message_handler message_handler_;
  void* user_data_;
  int wake_event_fd_;
  int epoll_fd_;
  bool init_;
  bool exit_;
  Mutex mutex_;
};

} /* namespace sup */

#endif /* _FY_HANDLER_HPP_ */
