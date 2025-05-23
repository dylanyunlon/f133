#ifndef _FY_ARRAY_H_
#define _FY_ARRAY_H_

#include <stdio.h>
#include <stdint.h>
#include <string>

typedef unsigned char byte;

namespace fy {

class array {
public:
  array() {
  }
  array(const byte* bytes, size_t len) {
    if (bytes && (len > 0)) {
      buf_.append((char*)bytes, len);
    }
  }
  virtual ~array() {
  }
  size_t Len() const {
    return buf_.length();
  }
  byte* Buf() const {
    return (byte*)buf_.data();
  }

  bool Append(byte byte1) {
    byte b[] = {byte1};
    return Append(b, sizeof(b));
  }
  bool Append(byte byte1, byte byte2) {
    byte b[] = {byte1, byte2};
    return Append(b, sizeof(b));
  }
  bool Append(byte byte1, byte byte2, byte byte3) {
    byte b[] = {byte1, byte2, byte3};
    return Append(b, sizeof(b));
  }
  bool Append(byte byte1, byte byte2, byte byte3, byte byte4) {
    byte b[] = {byte1, byte2, byte3, byte4};
    return Append(b, sizeof(b));
  }
  bool Append(byte byte1, byte byte2, byte byte3, byte byte4, byte byte5) {
    byte b[] = {byte1, byte2, byte3, byte4, byte5};
    return Append(b, sizeof(b));
  }
  bool Append(const void* bytes, size_t len) {
    if (bytes == NULL) {
      return false;
    }
    buf_.append((char*)bytes, len);
    return true;
  }
  bool Append16(uint16_t u16) {
    return Append(u16 >> 8, u16);
  }
  bool Append32(uint32_t u32) {
    return Append(u32 >> 24, u32 >> 16, u32 >> 8, u32);
  }

  void Clear() {
    buf_.clear();
  }

private:
  std::string buf_;
};

} /* namespace fy */

#endif /* _FY_ARRAY_H_ */
