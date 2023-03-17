// Copyright (C) 2023
//
// File:     public_define.h
// Brief:    Generic Macro Definition Module.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_PUBLIC_DEFINE_H_
#define TURBONET_SRC_BASE_PUBLIC_DEFINE_H_

#include <type_traits>
#include <cstdint>
#include <cstring>

namespace turbo {

// Primitive data types
using tb_s8 = char;
using tb_u8 = unsigned char;

using tb_s16 = short;
using tb_u16 = unsigned short;

using tb_s32 = int;
using tb_u32 = unsigned int;

using tb_s64_t = int64_t;
using tb_s64 = long long;
using tb_u64 = unsigned long long;

using tb_f32 = float;
using tb_f64 = double;

// Disable Copy and Assignment Constructors
#define DISALLOW_COPY_AND_ASSIGN(ClassName)  \
    ClassName (const ClassName&) = delete;   \
    ClassName operator=(const ClassName&) = delete;

// auto generated operator "!="
// derived must implement operator "="
template<typename T>
class EqualityComparable {
  friend bool operator!=(const T &lhs, const T &rhs) { return !lhs.EqualTo(rhs); };

  template<typename U = T>
  auto EqualTo(const T &rhs) const
  -> std::enable_if_t<std::is_same_v<decltype(std::declval<U>() == std::declval<U>()), bool>, bool> {
	return static_cast<const U &>(*this) == static_cast<const U &>(rhs);
  }
};

// auto generated operator "<=", ">", ">="
// derived must implement operator "<"
template<typename T>
class LessThanComparable {
  friend bool operator>(const T &lhs, const T &rhs) { return rhs < lhs; };
  friend bool operator<=(const T &lhs, const T &rhs) { return !(rhs < lhs); };
  friend bool operator>=(const T &lhs, const T &rhs) { return !(lhs < rhs); };

  template<typename U = T>
  auto LessThan(const T &rhs) const
  -> std::enable_if_t<std::is_same_v<decltype(std::declval<U>() == std::declval<U>()), bool>, bool> {
	return static_cast<const U &>(*this) < static_cast<const U &>(rhs);
  }
};

// Clear memory
inline void MemZero(void *p, size_t len) {
  memset(p, 0, len);
}

// Safe type conversion
template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

}
#endif //TURBONET_SRC_BASE_PUBLIC_DEFINE_H_