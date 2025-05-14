#ifndef _FY_FILES_HPP_
#define _FY_FILES_HPP_

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <string>
#include <vector>

#include "fy/strings.hpp"
#include "fy/format.hpp"
#include "fy/os.hpp"
#include "errors.hpp"

namespace fy {
namespace files {

inline bool remove(const char* file) {
  return unlink(file) == 0;
}

//文件存在返回真，否则假
inline bool exists(const char* path) {
    if (path == NULL) {
        return false;
    }
    if (access(path, F_OK) == 0) {
        return true;
    }
    return false;
}

inline bool create_script(const char *file, const char *content) {
  int fd = open(file, O_WRONLY|O_CREAT|O_EXCL, 0777);
  if (fd < 0) {
    return false;
  }

  ssize_t len = strlen(content);
  bool ret = (write(fd, content, len) == len);
  close(fd);

  return ret;
}

inline uint8_t* load_data(const char *file, uint32_t &size) {
  FILE *pf = fopen(file, "r");
  if (!pf) {
    return NULL;
  }

  uint8_t *data = NULL;
  do {
    fseek(pf, 0, SEEK_END);
    size = ftell(pf);
    fseek(pf, 0, SEEK_SET);

    data = (uint8_t *) malloc(size);
    if (!data) {
      break;
    }

    if (fread(data, 1, size, pf) != size) {
      free(data);
      data = NULL;
      size = 0;
    }
  } while (0);
  fclose(pf);

  return data;
}

inline bool save_data(const char *file, const void *data, uint32_t size) {
  FILE *pf = fopen(file, "w");
  if (!pf) {
    return false;
  }

  bool ret = false;
  if (fwrite(data, 1, size, pf) == size) {
    fflush(pf);
    ret = true;
  }

  fclose(pf);
  return ret;
}

// 文件大小
inline int64_t size_of(const std::string &file) {
    long filesize = -1;
    struct stat statbuff;
    if(stat(file.c_str(), &statbuff) < 0){
        return filesize;
    } else {
        filesize = statbuff.st_size;
    }
    return filesize;
}

// 从路径名中截取最后的文件名
inline std::string get_file_name(std::string path) {
	std::string::size_type pos = path.rfind("/");
	return path.substr(pos + 1, pos - path.rfind("."));
}

inline error copy_file(const std::string &source,
    const std::string &target,
    void *user_data,
    void (*progress)(const void* user_data, int64_t now, int64_t all)) {

  if (!exists(source.c_str())) {
    return error(fy::format("%s file not exist", source.c_str()));
  }
  FILE* in = fopen(source.c_str(), "rb");
  if (in == NULL) {
    return error(fy::format("failed to open file %s", source.c_str()));
  }
  FILE* out = fopen(target.c_str(), "wb");
  if (out == NULL) {
    return error(fy::format("failed to create file %s", target.c_str()));
  }

  int64_t all = size_of(source.c_str());
  if (all < 0) {
    return error("unknown file size");
  }
  int64_t now = 0;
  char buf[4096] = {0};
  int read_ret = 0;
  int write_ret = 0;

  if (progress) {
    progress(user_data, now, all);
  }
  while (!feof(in)) {
    read_ret = fread(buf, 1, sizeof(buf), in);
    if (read_ret < 0) {
      fclose(in);
      fclose(out);
      return error(fy::format("failed to read %s", source.c_str()));
    }
    write_ret = fwrite(buf, 1, read_ret, out);
    if (write_ret != read_ret) {
      fclose(in);
      fclose(out);
      return error(fy::format("failed to write %s", target.c_str()));
    }
    now += write_ret;
    if (progress) {
      progress(user_data, now, all);
    }
  }
  if (now != all) {
    return error("copy_file failed");
  }
  fflush(out);
  fclose(in);
  fclose(out);
  return nil;
}

inline bool _match(const std::string &str, const std::string &pattern) {
  if (pattern.compare("*") == 0) {
    return true;
  }
  std::string::size_type pos = pattern.find("*"); //*.mp3
  if (pos == 0) {
    return strings::endswith(str, pattern.substr(1, pattern.size() - 1));
  } else if (pos == (pattern.size() - 1)) {
    return strings::startswith(str, pattern.substr(0, pattern.size() - 1));
  }
  return true;
}

inline std::vector<std::string> list(const std::string &path, const std::string &pattern, bool recursive) {
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
      std::vector<std::string> sub = list(fy::path::join(path, entry->d_name), pattern, recursive);
      ret.insert(ret.end(), sub.begin(), sub.end());
    } else {
      if (!_match(entry->d_name, pattern)) {
        continue;
      }
      ret.push_back(fy::path::join(path, entry->d_name));
    }
  }
  closedir(d);
  return ret;
}

} /* namespace files */

} /* namespace fy */

#endif /* _FY_FILES_HPP_ */
