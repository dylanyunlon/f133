#ifndef _FY_LOG_HPP_
#define _FY_LOG_HPP_

#include <unistd.h>
#include <sys/syscall.h>

#include "thread.hpp"

namespace fy {

class logcat {
public:
  logcat(const std::string& file) {
    file_ = NULL;
    run_ = true;
    file_ = fopen(file.c_str(), "ab");
    if (file_ == NULL) {
      LOGD("failed to open %s", file.c_str());
      return;
    }
    fy::thread t(fy::logcat::logcat_thread, this);
    t.detach();
  }

  ~logcat() {
    run_ = false;
    LOGD("sup logcat stoped");
    if (file_) {
      fclose(file_);
    }
  }

public:
  static void logcat_thread(void *arg) {
    logcat* log = (logcat*)arg;
    FILE * p = popen("logcat -v time", "r");
    if (p) {
      while (!feof(p) && log->run_) {
        char buffer[1024] = { 0 };
        char* line = fgets(buffer, sizeof(buffer), p);
        if (feof(p) || ferror(p)) {
          break;
        }
        if (line) {
          fputs(line, log->file_);
          fflush(log->file_);
        }
        static int lines = 0;
        ++lines;
//        if (lines % 5 == 0) {
          sync();
//        }
      }
      fclose(p);
    } else {
      LOGD("open failed %d", errno);
    }
  }

private:
  FILE* file_;
  bool run_;
};


} /* namespace fy */

#endif /* _FY_LOG_HPP_ */
