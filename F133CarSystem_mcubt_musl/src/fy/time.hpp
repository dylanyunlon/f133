#ifndef _FY_TIME_H_
#define _FY_TIME_H_

#include <time.h>
#include <stdint.h>

#include <string>

#include "utils/TimeHelper.h"
#include "utils/Log.h"

namespace fy {

class time {
public:
  static time now() {
    return time();
  }

  // 开机累计毫秒
  static int64_t uptime() {
      struct timespec ts;
      clock_gettime(CLOCK_BOOTTIME, &ts);
      return int64_t(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
  }

  bool after(const time& t) {
    return true;
  }

  bool before(const time& t) {
    return true;
  }

  struct DateTime {
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
  };
  static struct DateTime GetDateTimeNow() {
    DateTime now;
    struct tm *t = TimeHelper::getDateTime();
    now.year = t->tm_year + 1900;
    now.mon = t->tm_mon + 1;
    now.day = t->tm_mday;
    now.hour = t->tm_hour;
    now.min = t->tm_min;
    now.sec = t->tm_sec;
    return now;
  }

private:

};

} /* namespace sup */

#endif /* _FY_TIME_H_ */
