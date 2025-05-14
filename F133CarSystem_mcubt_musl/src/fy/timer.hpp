#ifndef _FY_TIMER_H_
#define _FY_TIMER_H_

#include <time.h>
#include <map>
#include <memory.h>
#include <string.h>
#include <asm-generic/siginfo.h>

#include "mutex.hpp"

namespace fy {

/**
 * 计时timer
 */
class timer {
public:
  timer():add_up_time_(0), start_time_(this->uptime()) {
  }
  void reset() {
    start_time_ = this->uptime();
    add_up_time_ = 0;
  }
  int64_t elapsed() const {
    if (start_time_ < 0) {
      return add_up_time_;
    }
    return this->uptime() - start_time_ + add_up_time_;
  }
  void pause() {
    if (start_time_ >= 0) {
      add_up_time_ += (this->uptime() - start_time_);
    }
    start_time_ = -1;
  }
  void resume() {
    if (start_time_ < 0) {
      start_time_ = this->uptime();
    }
  }
  bool paused() const {
    return start_time_ < 0;
  }

private:
  static int64_t uptime() {
      struct timespec ts;
      clock_gettime(CLOCK_BOOTTIME, &ts);
      return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
  }
  int64_t add_up_time_;
  int64_t start_time_;
};

/**
 * 日志输出时间间隔
 */
class progress_timer : private fy::timer {
public:
  explicit progress_timer(const char* label):fy::timer(),label_(label) {
  }
  ~progress_timer(){
    int64_t progress = elapsed();
    int64_t second = progress/1000;
    if (second > 0) {
      LOGD("%s took \033[1;32m%llds.%lldms\033[0m", label_.c_str(), progress/1000, progress%1000);
    } else {
      LOGD("%s took \033[1;32m%lldms\033[0m", label_.c_str(),progress%1000);
    }
  }
private:
  std::string label_;
};

/**
 * 可能需要链接  -lrt
 */
class posix_timer {
public:
  typedef void (*posix_timer_callback)(int timer_id, void * user_data);

private:
  struct description;

public:
  virtual ~posix_timer() {
    std::map<int, description>::iterator des = all_timer_.begin();
    for (; des != all_timer_.end(); des++) {
      destroy(des->first);
    }
  }

private:
  /**
   * return timer id
   */
  static int _create(bool oneshot, int period_millis, int delay_millis, posix_timer_callback on_timer, void* user_data) {
    fy::lock_guard<fy::mutex> lock(instance()->mutex_);
    instance()->sival_int_++;
    int ret;
    timer_t timer;
    struct sigevent evp;
    struct timespec spec;
    struct itimerspec time_value;

    memset((void*)&evp, 0, sizeof(sigevent));
    evp.sigev_value.sival_int = instance()->sival_int_;
    /* 子线程运行 */
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = posix_timer::timer_thread_callback_wrapper;

    ret = timer_create(CLOCK_MONOTONIC, &evp, &timer);
    if (-1 == ret) {
      //LOGE("timer_create Failed");
      return -1;
    }

    time_value.it_interval.tv_sec = period_millis / 1000;      /*n秒触发一次*/
    time_value.it_interval.tv_nsec = period_millis % 1000 * 1000 * 1000;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    time_value.it_value.tv_sec = spec.tv_sec + delay_millis / 1000;      /*n秒后启动*/
    time_value.it_value.tv_nsec = spec.tv_nsec + delay_millis % 1000 * 1000 * 1000;

    struct description des = {
        oneshot,
        period_millis,
        timer,
        instance()->sival_int_,
        user_data,
        on_timer
    };
    instance()->all_timer_[instance()->sival_int_] = des;

    //启动
    ret = timer_settime(timer, CLOCK_MONOTONIC, &time_value, NULL);
    if(-1 == ret) {
      //LOGE("timer_settime Failed");
      timer_delete(timer);
      instance()->all_timer_.erase(instance()->all_timer_.find(instance()->sival_int_));
      return -2;
    }
    //LOGD("start timer");
    return instance()->sival_int_;
  }

public:
  static void destroy(int timer_id) {
    fy::lock_guard<fy::mutex> lock(instance()->mutex_);
    std::map<int, description>::iterator it = instance()->all_timer_.find(timer_id);
    if (it == instance()->all_timer_.end()) {
      return;
    }
    timer_delete(it->second.timer);
    instance()->all_timer_.erase(it);
  }

  static int oneshot(time_t delay_millis, posix_timer_callback on_timer, void* user_data) {
    return instance()->_create(true, 0, delay_millis, on_timer, user_data);
  }

  static int create(int period_millis, int delay_millis, posix_timer_callback on_timer, void* user_data) {
    return instance()->_create(false, period_millis, delay_millis, on_timer, user_data);
  }

private:
  posix_timer() {
    sival_int_ = 0;
  }

  static posix_timer* instance() {
    static posix_timer singleton;
    return &singleton;
  }

  static void timer_thread_callback_wrapper(union sigval v) {
    instance()->timer_thread_callback(v);
  }

  void timer_thread_callback(union sigval v) {
    description des;
    {  // scope lock
      fy::lock_guard<fy::mutex> lock(mutex_);
      std::map<int, description>::iterator it = all_timer_.find(v.sival_int);
      if (it == all_timer_.end()) {
        return;
      }
      des = it->second;
      if (des.is_oneshot) {
        timer_delete(it->second.timer);
        instance()->all_timer_.erase(it);
      }
    }
    des.callback(des.sival_int, des.user_data);
  }

  struct description {
    bool is_oneshot;
    time_t interval;
    timer_t timer;
    int sival_int;
    void* user_data;
    posix_timer_callback callback;
  };
private:
  int sival_int_;
  fy::mutex mutex_;
  std::map<int, description> all_timer_;

};

} /* namespace fy */

#endif /* JNI_FY_TIMER_H_ */
