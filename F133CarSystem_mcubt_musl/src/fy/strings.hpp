#pragma once

#include <ctype.h>
#include <string>
#include <vector>

namespace fy {

namespace strings {

inline std::string join(std::string val) {
  return val;
}
template<class ...ARGS>
inline std::string join(std::string val, ARGS ...args) {
  return val + my_sum(args...);
}

inline bool endswith(const std::string& str, const std::string& end) {
  int srclen = str.size();
  int endlen = end.size();
  if(srclen >= endlen) {
    std::string temp = str.substr(srclen - endlen, endlen);
    if(temp == end)
      return true;
  }
  return false;
}

inline bool startswith(const std::string& str, const std::string& start) {
  int srclen = str.size();
  int startlen = start.size();
  if(srclen >= startlen) {
    std::string temp = str.substr(0, startlen);
    if(temp == start)
      return true;
  }
  return false;
}

inline std::string trimsuffix(const std::string &str, const std::string &suffix) {
  size_t p = str.find_last_of(suffix);
  if (p != (str.size() - suffix.size())) {
    return str;
  }
  return str.substr(0, p);
}

inline std::string trimprefix(const std::string &str, const std::string &prefix) {
  if (startswith(str, prefix)) {
    return str.substr(prefix.size());
  }
  return str;
}

inline std::string replace(const std::string& str, const std::string &old_str, const std::string& new_str) {
  std::string::size_type pos = 0;
  if (old_str.empty()) {
    return str;
  }
  std::string temp = str;
  while (std::string::npos != (pos = temp.find(old_str))) {
    temp = temp.replace(pos, old_str.size(), new_str);
  }
  return temp;
}

/*  UTF - 8，是一种变长编码，具体规则如下：
    1）对于单字节的符号，字节的第一位设为0，后面7位为这个符号的unicode码。因此对于英语字母，UTF
   - 8编码和ASCII码是相同的。 2）对于n字节的符号（n >
   1），第一个字节的前n位都设为1，第n +
   1位设为0，后面字节的前两位一律设为10。剩下的没有提及的二进制位，全部为这个符号的unicode码。

    如表 :
    1字节 0xxxxxxx
    2字节 110xxxxx 10xxxxxx
    3字节 1110xxxx 10xxxxxx 10xxxxxx
    4字节 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    5字节 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    6字节 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
*/

// 将string中的中文分解成单个字, 适用于UTF-8编码
inline void split_word(const std::string& words, std::vector<std::string>* characters) {
  int num = words.size();
  int i = 0;
  while (i < num) {
    int size = 1;
    if (words[i] & 0x80) {
      char temp = words[i];
      temp <<= 1;
      do {
        temp <<= 1;
        ++size;
      } while (temp & 0x80);
    }
    characters->push_back(words.substr(i, size));
    i += size;
  }
}

inline void split(const std::string& str, const std::string& delimitor,
    std::vector<std::string>* group) {
  if (group == NULL) {
    return;
  }
  if (str.empty()) {
    return;
  }
  if (delimitor.empty()) {
    group->push_back(str);
    return;
  }
  std::string::size_type begin = 0;
  while (true) {
    auto pos = str.find(delimitor, begin);
    if (pos == str.npos) {
      break;
    }
    if (pos > begin) {
      group->push_back(str.substr(begin, pos - begin));
    }
    pos += (int) delimitor.size();
    begin = pos;
  }
  if (begin < str.size()) {
    group->push_back(str.substr(begin));
  }
}

inline std::string upper(const std::string& text) {
  std::string dest = text;
  char* p = (char*)dest.data();
  size_t length = dest.length();
  for (size_t i = 0; i < length; ++i) {
    int c = (unsigned char)p[i];
    p[i] = toupper(c);
  }
  return dest;
}

inline std::string lower(const std::string& text) {
  std::string dest = text;
  char* p = (char*)dest.data();
  size_t length = dest.length();
  for (size_t i = 0; i < length; ++i) {
    int c = (unsigned char)p[i];
    p[i] = tolower(c);
  }
  return dest;
}

} //namespace strings
} //namespace sup
