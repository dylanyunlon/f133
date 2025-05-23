#ifndef _FY_FORMAT_HPP_
#define _FY_FORMAT_HPP_

#include <stdarg.h>
#include <stdio.h>
#include <string>

namespace fy {

inline std::string format(const char* format, ...) {
  std::string tmp;
  va_list vl;
  va_start(vl, format);
  size_t num = vsnprintf(0, 0, format, vl);
  if (num >= tmp.capacity()) tmp.reserve(num + sizeof(char));

  tmp.resize(num);
  vsnprintf((char*)tmp.data(), tmp.capacity(), format, vl);
  va_end(vl);
  return tmp;
}

} /* namespace fy */

#endif /* _FY_FORMAT_HPP_ */
