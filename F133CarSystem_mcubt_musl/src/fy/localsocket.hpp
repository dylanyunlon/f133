/*
 * localsocket.hpp
 *
 *  Created on: 2024年3月26日
 *      Author: guoxs
 */

#ifndef _FY_LOCALSOCKET_HPP_
#define _FY_LOCALSOCKET_HPP_

#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "asio.hpp"
#include "utils/Log.h"

namespace fy {

class localsocket {
public:
  localsocket() : fd_(-1) {

  }

  ~localsocket() {
    close();
  }

  bool connect(const char *addr) {
    if (fd_ >= 0) {
      LOGD("localsocket has connected\n");
      return true;
    }

    int fd = ::socket(PF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
      LOGE("localsocket create socket err: %s\n", strerror(errno));
      return false;
    }

    struct sockaddr_un saddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, addr);

    if (::connect(fd, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
      LOGE("localsocket can't connect to %s err: %s\n", addr, strerror(errno));
      ::close(fd);
      return false;
    }

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
      LOGE("localsocket get flags err: %s\n", strerror(errno));
      ::close(fd);
      return false;
    }

    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) {
      LOGE("localsocket set flags err: %s\n", strerror(errno));
      ::close(fd);
      return false;
    }

    fd_ = fd;
    return true;
  }

  void close() {
    if (fd_ >= 0) {
      ::close(fd_);
      fd_ = -1;
    }
  }

  int write(const void *data, size_t len) {
    if (fd_ < 0) {
      return -1;
    }

    return ::write(fd_, data, len);
  }

  int read(void *data, size_t len, int timeout_millis) {
    if (fd_ < 0) {
      return -1;
    }

    if (fy::asio::readable(fd_, timeout_millis) <= 0) {
      LOGE("localsocket readable err\n");
      return -1;
    }

    return ::read(fd_, data, len);
  }

private:
  int fd_;
};

}

#endif /* _FY_LOCALSOCKET_HPP_ */
