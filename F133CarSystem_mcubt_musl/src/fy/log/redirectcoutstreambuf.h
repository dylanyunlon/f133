#ifndef FLY_LOG_REDIRECTCOUTSTREAMBUF_H_
#define FLY_LOG_REDIRECTCOUTSTREAMBUF_H_

/**
 * 重定向std::cout 到 Android Log
 *
 * 使用方法：
 * #include <sup/log/redirectcoutstreambuf.h
 * fly::log::RedirectCoutStreamBuf::Redirect("TAG");
 *
 */
#include <memory.h>
#include <iostream>
#include <streambuf>

#ifdef ANDROID
#include <android/log.h>
#endif

namespace fy {
namespace log {

class RedirectCoutStreamBuf: public std::streambuf {
  enum {
    BUFFER_SIZE = 2047,
  };

public:
  RedirectCoutStreamBuf(const char* tag) {
    buffer_[BUFFER_SIZE] = '\0';
    setp(buffer_, buffer_ + BUFFER_SIZE - 1);
    memset(tag_, 0, sizeof(tag_));
    memcpy(tag_, tag, strlen(tag));
  }

  ~RedirectCoutStreamBuf() {
    sync();
  }

  static void Redirect(const char* tag) {
    static RedirectCoutStreamBuf singleton(tag);
    std::cout.rdbuf(&singleton);
  }

protected:
  virtual int_type overflow(int_type c) {
    if (c != EOF) {
      *pptr() = c;
      pbump(1);
    }
    flush_buffer();
    return c;
  }

  virtual int sync() {
    flush_buffer();
    return 0;
  }

private:
  int flush_buffer() {
    int len = int(pptr() - pbase());
    if (len <= 0)
      return 0;

    if (len <= BUFFER_SIZE)
      buffer_[len] = '\0';

#ifdef ANDROID
    android_LogPriority t = ANDROID_LOG_INFO;
    __android_log_write(t, tag_, buffer_);
#else
    printf("%s", buffer_);
#endif

    pbump(-len);
    return len;
  }

private:
  char buffer_[BUFFER_SIZE + 1];
  char tag_[32];
};
}
}

#endif /* FLY_LOG_REDIRECTCOUTSTREAMBUF_H_ */
