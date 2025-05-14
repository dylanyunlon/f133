#ifndef _FY_THREAD_H_
#define _FY_THREAD_H_

#include <sys/prctl.h>
#include <string>
#include <pthread.h>
#include <errno.h>
#include <utils/Log.h>

namespace fy {

class thread {
public:
  typedef void (*func_thread_void)();
  typedef void (*func_thread_arg1)(void* arg1);

  struct meta_data {
    func_thread_arg1 func;
    void* arg;
  };

  thread(fy::thread::func_thread_arg1 func, void* arg) {
    int ret;
    joinable_ = true;
    struct meta_data* meta = new meta_data();
    meta->func = func;
    meta->arg = arg;
    ret = pthread_create(&thread_id_, NULL, fy::thread::func_thread_wrapper, meta);
    if (ret != 0) {
      LOGE("create thread error, errno = %d", errno);
    }
  }

  static std::string name() {
    char name[64] = {0};
    prctl(PR_GET_NAME, (unsigned long)name);
    return name;
  }

  void set_name(const char* name64) {
    prctl(PR_SET_NAME, (unsigned long)name64);
  }

  void join() {
    void* ret_val;
    int ret = pthread_join(thread_id_, &ret_val);
    if (ret != 0) {
      LOGE("pthread_join failed errno %d", errno);
    }
    joinable_ = false;
  }

  void detach() {
    int ret = pthread_detach(thread_id_);
    if (ret != 0) {
      LOGE("pthread_detach failed, ret = %d, errno = %d", ret, errno);
    }
    joinable_ = false;
  }

  bool joinable() {
    return joinable_;
  }

  virtual ~thread() {
    if (joinable()) {
      detach();
      int ret_val;
      pthread_exit(&ret_val);
    }
  }

private:
  static void* func_thread_wrapper(void *arg) {
    fy::thread::meta_data* meta = (fy::thread::meta_data*)arg;
    meta->func(meta->arg);
    delete meta;
    return NULL;
  }

  pthread_t thread_id_;
  bool joinable_;
};

inline bool run_thread(void* (*thread_cb) (void *), void *arg = NULL) {
  pthread_t th;
  int ret = pthread_create(&th, NULL, thread_cb, arg);
  if (ret != 0) {
    LOGE("create thread error, errno = %d", errno);
    return false;
  }
  pthread_detach(th);
  return true;
}

} /* namespace sup */

#endif
