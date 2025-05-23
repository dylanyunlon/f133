#ifndef _FY_ERRORS_HPP_
#define _FY_ERRORS_HPP_

#include <string>

namespace fy {

class error {
public:
  error(const std::string& what):what_(what.c_str()) {
  }
  std::string what() const {
    return what_;
  }
  bool operator!=(const error& e) const {
    return e.what_.compare(what_) != 0;
  }
  bool operator==(const error& e) const {
    return e.what_.compare(what_) == 0;
  }
private:
  std::string what_;
};

} /* namespace fy */


#define nil error("nil")
#define error_timeout error("timeout")

#endif /* _FY_ERRORS_HPP_ */
