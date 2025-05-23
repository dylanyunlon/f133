/*
 * common.h
 *
 *  Created on: 2023年1月3日
 *      Author: ZKSWE Develop Team
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <set>
#include "config.h"
#include "os/SystemProperties.h"
#include "entry/EasyUIContext.h"
#include "utils/Log.h"
#include "manager/LanguageManager.h"

#define BIT(n)                 (1 << (n))
#define BITS_CHECK(v, b)       (v & (b))
#define BITS_SET(v, b)         v |= (b)
#define BITS_CLEAR(v, b)       v &= ~(b)

#define TAB_SIZE(t)            (sizeof(t)/sizeof(t[0]))
#define DELAY(ms)              usleep(ms * 1000)
#define FILE_EXIST(f)          (access(f, F_OK) == 0)
#define CHECK_CP(src, dst)     if (!FILE_EXIST(dst)) { system("cp " src " " dst); }

#define SET_FOREACH(s, t, v)   for (std::set<t>::iterator v = s.begin(); v != s.end(); ++v)

#define RETURN_IF_FAIL(c)             if (!(c)) { return; }
#define RETURN_VALUE_IF_FAIL(c, v)    if (!(c)) { return (v); }

#define WAIT(cond, count, delay)               \
    do {                                       \
        int _count = 0;                        \
        while (!(cond) && (_count < count)) {  \
            _count++;                          \
            DELAY(delay);                      \
        }                                      \
    } while (0)

#define START_SERVICE(name)    SystemProperties::setString("ctl.start", name)
#define RESTART_SERVICE(name)  SystemProperties::setString("ctl.restart", name)
#define STOP_SERVICE(name)     SystemProperties::setString("ctl.stop", name)

#define OPEN_APP(name)         EASYUICONTEXT->openActivity(name"Activity")
#define CLOSE_APP(name)        EASYUICONTEXT->closeActivity(name"Activity")

#define LTOV(s)        		   LANGUAGEMANAGER->getValue(s)

#define DELETE_COPY_AND_ASSIGN_METHOD(Class) \
  Class(const Class&) = delete; \
  Class& operator=(const Class&) = delete


#define DECLARE_SINGLETON_PATTERN(Class) \
public:                      \
  static Class& instance() { \
    static Class singleton;  \
    return singleton;        \
  }                          \
private:                     \
  Class(){}

#endif /* _COMMON_H_ */
