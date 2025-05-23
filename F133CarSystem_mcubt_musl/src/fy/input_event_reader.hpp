#ifndef _FY_INPUT_EVENT_READER_HPP_
#define _FY_INPUT_EVENT_READER_HPP_

#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <linux/input.h>
#include <fcntl.h>

#include <string>
#include <vector>

#include <fy/mutex.hpp>

namespace fy {

class input_event_reader {
private:
  struct input_source {
    std::string name;
    std::string path;
    int fd;
  };

public:

  explicit input_event_reader() {
  }

  virtual ~input_event_reader() {
    lock_guard<fy::mutex> lock(mutex_);
    for (std::vector<input_source>::iterator it = input_sources_.begin(); it != input_sources_.end(); ++it) {
      if (it->fd >= 0) {
        close(it->fd);
      }
    }
  }

  int add_input(const std::string& name, const std::string& input_path) {
    lock_guard<fy::mutex> lock(mutex_);
    int fd = open(input_path.c_str(), O_RDONLY);
    if (fd < 0) {
      return -1;
    }
    struct input_source source;
    source.name = name;
    source.path = input_path;
    source.fd = fd;
    input_sources_.push_back(source);
    return 0;
  }

  void remove_input(const std::string& name) {
    lock_guard<fy::mutex> lock(mutex_);
    for (std::vector<input_source>::iterator it = input_sources_.begin(); it != input_sources_.end(); ++it) {
      if (name.compare(it->name) == 0) {
        if(it->fd >= 0) {
          close(it->fd);
        }
        input_sources_.erase(it);
        return;
      }
    }
  }

  /**
   * On success > 0
   * Timeout  0
   * Failed < 0
   */
  int read(std::string* name, input_event* event, int timeout_millis) {
    fd_set read_fds;
    int max_fd = setup_fdset(&read_fds);
    if (max_fd < 0) {
      return -1;
    }

    struct timeval tv;
    tv.tv_sec = timeout_millis / 1000;
    tv.tv_usec = timeout_millis % 1000;
    int ret = select(max_fd + 1, &read_fds, NULL, NULL, &tv);
    if (ret < 0) {
        //LOGD("error");
        return -2;
    } else if (ret == 0) {
      //timeout
      return 0;
    } else {
      for (std::vector<input_source>::iterator it = input_sources_.begin(); it != input_sources_.end(); ++it) {
        if (it->fd < 0) {
          continue;
        }
        *name = it->name;
        if(FD_ISSET(it->fd, &read_fds)) {
          int rs = ::read(it->fd, event, sizeof(input_event));
          if (rs == sizeof(input_event)) {
            return 1;
          } else {
            //LOGD("read exception");
            return -3;
          }
        }
      }
    }
    return 0;
  }

protected:
  int setup_fdset(fd_set* set) {
    FD_ZERO(set);
    int max_fd = -1;
    for (std::vector<input_source>::iterator it = input_sources_.begin(); it != input_sources_.end(); ++it) {
      int fd = it->fd;
      if (fd >= 0) {
        FD_SET(fd, set);
        if (fd > max_fd) {
          max_fd = fd;
        }
      }
    }
    if (max_fd < 0) {
      return -1;
    }
    return max_fd;
  }

private:
  std::vector<input_source> input_sources_;
  fy::mutex mutex_;
};

} /* namespace fy */

#endif /* _FY_INPUT_EVENT_READER_HPP_ */
