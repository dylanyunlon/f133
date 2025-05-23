#ifndef _FY_ASIO_H_
#define _FY_ASIO_H_

#include <poll.h>
#include <errno.h>
#include <string.h>

namespace fy {

namespace asio {

/**
 * -1 error
 * 0 timeout
 * >0 get event
 */
inline int readable(int fd, int timeout_ms) {
  int ret = -1;
  struct pollfd pfd;
  memset(&pfd, 0, sizeof(pfd));
  pfd.fd = fd;
  pfd.events = POLLIN;

  // retry again for EINTR
  for (int i = 0; i < 2; i++) {
    ret = ::poll(&pfd, 1, timeout_ms);
    if (-1 == ret && EINTR == errno)
      continue;
    break;
  }
  return ret;
}

/**
 * -1 error
 * 0 timeout
 * >0 get event
 */
inline int writable(int fd, int timeout_ms) {
  int ret = -1;
  struct pollfd pfd;
  memset(&pfd, 0, sizeof(pfd));
  pfd.fd = fd;
  pfd.events = POLLOUT;
  // retry again for EINTR
  for (int i = 0; i < 2; i++) {
    ret = ::poll(&pfd, 1, timeout_ms);
    if (-1 == ret && EINTR == errno)
      continue;
    break;
  }
  return ret;
}

} /* namespace asio */
} /* namespace sup */

#endif
