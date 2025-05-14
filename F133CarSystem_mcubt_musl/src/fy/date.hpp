/*
 * date.hpp
 *
 *  Created on: 2023年11月13日
 *      Author: ZKSWE Develop Team
 */

#ifndef _FY_DATE_H_
#define _FY_DATE_H_

namespace fy {
namespace date {

// 是否是闰年
inline bool is_leap_year(int year) {
  return ((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0));
}

// 获取某年某月的天数
inline int get_days(int year, int month) {
  if ((month == 2) && is_leap_year(year)) {	// 二月
    return 29;
  }

  int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  return days[month - 1];
}

// 判断某个日期是星期几的函数
inline int get_week_day(int year, int month, int day) {
  int i, temp, weekday, days = 0;
  for (i = 1; i <= month - 1; i++) {
    days += get_days(year, i);
  }
  days += day;
  temp = (year - 1) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + days;
  weekday = temp % 7;

  return weekday;
}

}
}

#endif /* _FY_DATE_H_ */
