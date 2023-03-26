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
#include <string>
#include <concepts>
#include <csignal>

namespace turbo {

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
  requires requires(U &&u){
	{
	std::declval<U>() == std::declval<U>()
	}
	-> std::convertible_to<bool>;
  }
  auto EqualTo(const T &rhs) const {
	return static_cast<const U &>(*this) == static_cast<const U &>(rhs);
  }
};

// auto generated operator "<=", ">", ">="
// derived must implement operator "<"
template<typename T>
class LessThanComparable {
  friend bool operator>(const T &lhs, const T &rhs) { return rhs.LessThan(lhs); };
  friend bool operator<=(const T &lhs, const T &rhs) { return !(rhs.LessThan(lhs)); };
  friend bool operator>=(const T &lhs, const T &rhs) { return !(lhs.LessThan(rhs)); };

  template<typename U = T>
  requires requires(U &&u){
	{
	std::declval<U>() == std::declval<U>()
	}
	-> std::convertible_to<bool>;
  }
  auto LessThan(const T &rhs) const {
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

// TODO: create ProcessInfo implement it.
inline std::string GetHostName() {
  char host_name[256];
  if (::gethostname(host_name, sizeof(host_name)) == 0)
	host_name[sizeof(host_name) - 1] = '\0';
  else
	return "Unknown Host";
  return host_name;
}

// TODO: create ProcessInfo implement it.
inline std::string GetPid(){
  return std::to_string(::getpid());
}

static const long long kMicroSecondsPerSecond = 1000 * 1000;
static const long long kNanoSecondsPerSecond = 1000 * 1000 * 1000;

}
#endif //TURBONET_SRC_BASE_PUBLIC_DEFINE_H_