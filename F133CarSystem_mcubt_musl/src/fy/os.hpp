#ifndef __FY_OS_H__
#define __FY_OS_H__

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <string>
#include <vector>
#include "fy/format.hpp"
#include "fy/strings.hpp"

#include "security/SecurityManager.h"
#include "utils/Log.h"

namespace fy {

/**
 * 小端返回true，大端false
 */
inline bool is_little_endian() {
  union {
    int a;
    char b;
  } c;
  c.a = 1;
  return (c.b == 1);
}

template<typename T>
inline void swap(T* a, T* b) {
  *a = (*a) ^ (*b);
  *b = (*a) ^ (*b);
  *a = (*a) ^ (*b);
}

template<typename T>
inline void reverse_endian(T* v) {
  typedef unsigned char byte;
  byte* b = (byte*) v;
  for (int i = 0; i < sizeof(T) / 2; ++i) {
    swap(&b[i], &b[sizeof(T) - 1 - i]);
  }
}

template<typename T>
inline T from_endian(T val, bool le) {
  static bool e = is_little_endian();
  if (e != le) {
    reverse_endian(&val);
    return val;
  }
  return val;
}

inline void system(const std::string &command, std::string* output) {
  FILE *p = popen(command.c_str(), "r");
  if (p) {
    while (!feof(p)) {
      char buffer[1024] = { 0 };
      char *line = fgets(buffer, sizeof(buffer), p);
      if (line && output) {
        *output += buffer;
      }
      if (feof(p) || ferror(p)) {
        break;
      }
    }
    pclose(p);
  }
}

inline std::string system(const std::string &command) {
  std::string out;
  system(command, &out);
  return out;
}

inline void pkill(const std::string& name) {
  std::string out;
  fy::system("ps", &out);
  std::vector<std::string> lines;
  fy::strings::split(out, "\n", &lines);
  for (auto line : lines) {
    if (line.find(name) == line.npos) {
      continue;
    }
    int pid;
    if (1 != sscanf(line.c_str(), "%*[ ]%d %*s", &pid)) {
      continue;
    }

    if(::system(fy::format("kill -9 %d", pid).c_str())){
    }
    break;
  }
}

// 释放cache内存
inline void drop_caches() {
  FILE *pf = fopen("/proc/sys/vm/drop_caches", "w");
  if (pf) {
    fwrite("3", 1, 1, pf);
    fclose(pf);
  }
}

// 可用内存,单位:字节
inline size_t free_ram() {
  struct sysinfo si;
  return (sysinfo(&si) == 0) ? si.freeram : 0;
}

// 总内存,单位:字节
inline size_t total_ram() {
  struct sysinfo si;
  return (sysinfo(&si) == 0) ? si.totalram : 0;
}

inline void kill_process(const char *name) {
  DIR *dir = opendir("/proc");
  if (!dir) {
    return;
  }

  struct dirent *de;
  while ((de = readdir(dir))) {
    if (!isdigit(de->d_name[0])) {
      continue;
    }

    char stat[32];
    sprintf(stat, "/proc/%s/stat", de->d_name);
    FILE *pf = fopen(stat, "r");
    if (!pf) {
      continue;
    }

    char buf[64] = { 0 };
    fread(buf, 1, 63, pf);
    fclose(pf);

    char keyword[64];
    sprintf(keyword, "(%s)", name);

    if (strstr(buf, keyword)) {
      sprintf(buf, "kill %s", de->d_name);
      system(buf);
      LOGD("%s %s\n", buf, name);
      break;
    }
  }

  closedir(dir);
}

inline std::string gen_uuid_str() {
  uint8_t id[16] = { 0 };
  uint16_t ret = 0;
  SECURITYMANAGER->getDevID(id);
  for (int i = 0; i < 16; i += 2) {
    ret += *(uint16_t *) (id + i);
  }

  char uuid_str[16];
  sprintf(uuid_str, "_%04X", ret);

  return std::string(uuid_str);
}

inline bool gen_conf_file(const char *src, const char *dst, const char *key_str) {
  bool ret = false;
  FILE *src_file = NULL;
  FILE *dst_file = NULL;
  char *content = NULL;
  size_t file_size;
  const char *key_pos;

  if (access(dst, F_OK) == 0) {
    return true;
  }

  src_file = fopen(src, "r");
  if (!src_file) {
    LOGE("open %s err\n", src);
    goto END;
  }

  dst_file = fopen(dst, "w");
  if (!dst_file) {
    LOGE("open %s err\n", dst);
    goto END;
  }

  fseek(src_file, 0, SEEK_END);
  file_size = ftell(src_file);
  fseek(src_file, 0, SEEK_SET);

  content = (char *) malloc(file_size + 1);
  if (!content) {
    goto END;
  }

  if (fread(content, 1, file_size, src_file) != file_size) {
    goto END;
  }
  content[file_size] = '\0';

  if ((key_pos = strstr(content, key_str)) != NULL) {
    key_pos += strlen(key_str);

    if (fwrite(content, 1, key_pos - content, dst_file) != (size_t) (key_pos - content)) {
      goto END;
    }

    std::string suffix_str = std::string("_") + gen_uuid_str();
    if (fwrite(suffix_str.c_str(), 1, suffix_str.size(), dst_file) != suffix_str.size()) {
      goto END;
    }

    if (fwrite(key_pos, 1, strlen(key_pos), dst_file) != strlen(key_pos)) {
      goto END;
    }
  } else {
    if (fwrite(content, 1, file_size, dst_file) != file_size) {
      goto END;
    }
  }

  ret = true;

END:
  free(content);
  if (src_file) {
    fclose(src_file);
  }
  if (dst_file) {
    fflush(dst_file);
    fclose(dst_file);
  }
  if (!ret) {
    remove(dst);
  }

  return ret;
}

class statfs {
public:
  explicit statfs(const char* mount_point_path) {
      memset(&stat_, 0, sizeof(stat_));
      if ((error_ = ::statfs(mount_point_path, &stat_)) != 0) {
        error_ = errno;
      }
  }
  uint64_t block_count() {
    return stat_.f_blocks;
  }

  uint64_t block_size() {
    return stat_.f_bsize;
  }

  uint64_t free_blocks() {
    return stat_.f_bfree;
  }

  uint64_t total_bytes() {
    return stat_.f_blocks * stat_.f_bsize;
  }

  uint64_t available_bytes() {
    return stat_.f_bavail * stat_.f_bsize;
  }

  // 0 ok, other error
  int error() {
    return error_;
  }

private:
  struct ::statfs stat_;
  int error_;
};


namespace path {

//取路径前缀
inline std::string prefix(const std::string &path) {
  size_t pos = path.find_last_of(".");
  if (pos == std::string::npos) {
    return path;
  }
  return path.substr(0, pos);
}

//取文件后缀
inline std::string suffix(const std::string &path) {
  size_t pos = path.find_last_of(".");
  if (pos == std::string::npos) {
    return path;
  }
  if ((path.size() - 1) == pos) {
    return "";
  }
  return path.substr(pos + 1);
}

inline std::string join(const std::string &a, const std::string &b) {
  if (a.empty()) {
    return b;
  }
  if (a[a.size() - 1] == '/') {
    return a + b;
  }
  return a + "/" + b;
}

inline std::string dir(const std::string &path) {
  size_t pos = path.find_last_of("/");
  if (pos == 0) {
    return "/";
  }
  if (pos != std::string::npos) {
    return path.substr(0, pos);
  }
  return "";
}

inline std::string base(const std::string &path) {
  size_t pos = path.find_last_of("/");
  if (pos != std::string::npos) {
    return path.substr(pos + 1);
  }
  return path;
}

inline std::vector<std::string> ls(const std::string path, const std::string &sscanf_format = "", bool recursive = false, bool only_name = true) {
  // dir/*.cpp
  std::vector<std::string> ret;
  DIR *d;
  struct dirent *entry;
  if ((d = opendir(path.c_str())) == NULL) {
    //can't open dir
    return ret;
  }
  while ((entry = readdir(d)) != NULL) {
    if ((strcmp(".", entry->d_name) == 0) || (strcmp("..", entry->d_name) == 0)) {
      continue;
    }

    if ((entry->d_type == DT_DIR) && recursive) {
      std::vector<std::string> sub = ls(join(path, entry->d_name), sscanf_format, recursive);
      ret.insert(ret.end(), sub.begin(), sub.end());
    } else {
      ret.push_back(only_name ? std::string(entry->d_name) : join(path, entry->d_name));
    }
  }
  closedir(d);
  return ret;
}

inline std::vector<std::string> available_serial_port() {
  std::vector<std::string> v;
  auto list = ls("/dev");
  for (auto i : list) {
    if (i.find("ttyS") != std::string::npos) {
      v.push_back(std::string("/dev/") + i);
    }
  }
  return v;
}

inline bool mkdir(const char *dir) {
  char path[512] = { 0 };
  char *name = path;
  const char *start = dir, *end = NULL;
  int len;

  while (start && *start) {
    end = strchr(start, '/');
    if (end) {
      end++;
      len = end - start;
      strncpy(name, start, len);
      name += len;
    } else {
      strcpy(name, start);
    }

    if (access(path, F_OK) != 0) {
      if (::mkdir(path, 0755) != 0) {
        return false;
      }
    }

    start = end;
  }

  return true;
}

} // namespace path

namespace os {

inline int open_uart(const char *dev, uint32_t baudrate) {
  int fd = open(dev, O_RDWR|O_NOCTTY|O_NONBLOCK);
  if (fd < 0) {
    LOGE("open uart dev %s fail, %s\n", dev, strerror(errno));
    return -1;
  }

  struct termios newtio = { 0 };
  newtio.c_cflag = baudrate|CS8|CLOCAL|CREAD;
  newtio.c_iflag = 0;	// IGNPAR | ICRNL
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;	// ICANON
  newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
  newtio.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
  tcflush(fd, TCIOFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  LOGD("open uart dev %s ok\n", dev);

  return fd;
}

inline int create_pty(const char *dev) {
  int fd = open("/dev/ptmx", O_RDWR);
  if (fd < 0) {
    LOGE("open /dev/ptmx failed, errno=%d(%s)\n", errno, strerror(errno));
    return -1;
  }

  grantpt(fd);
  unlockpt(fd);

  const char *pts = ptsname(fd);
  if (!pts) {
    LOGE("ptsname fail\n");
    close(fd);
    return -1;
  }

  unlink(dev);
  if (symlink(pts, dev) < 0) {
    LOGE("create %s failed, errno=%d(%s)\n", dev, errno, strerror(errno));
    close(fd);
    return -1;
  }

  return fd;
}

} // namespace os

} // namespace fy

#endif /* __FY_OS_H__ */
