#ifndef _FY_CHRONO_HPP_
#define _FY_CHRONO_HPP_

#include <time.h>
#include <string>
#include "utils/Log.h"

namespace fy {
namespace chrono {

class system_clock {
public:

  class time_point {
  public:
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
    int week;
    int msec;
  };

  static time_point now() {
    time_point tp;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    tp.msec = ts.tv_nsec / 1000000;
    struct tm *t = gmtime(&ts.tv_sec);
    tp.year = t->tm_year + 1900;
    tp.mon = t->tm_mon + 1;
    tp.day = t->tm_mday;
    tp.hour = t->tm_hour;
    tp.min = t->tm_min;
    tp.sec = t->tm_sec;
    tp.week = t->tm_wday;
    return tp;
  }

};


}
}

#endif
