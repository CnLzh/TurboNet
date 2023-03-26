// Copyright (C) 2023
//
// File:     singleton.h
// Brief:    Singleton class.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_SINGLETON_H_
#define TURBONET_SRC_BASE_SINGLETON_H_

#include "public_define.h"

#include <cassert>
#include <memory>
#include <mutex>

namespace turbo {

template<typename T>
concept HasNoDestroy = requires{
  typename T;
  { &T::NoDestroy } ->std::same_as<void (*)()>;
};

template<typename T>
class Singleton {
 public:
  Singleton() = delete;
  ~Singleton() = delete;

  template<typename... Args>
  static T &Instance(Args &&... args) {
	std::call_once(ponce_,
				   [&]() {
					 value_ = std::make_unique<T>(std::forward<Args>(args)...);
					 if constexpr (!HasNoDestroy<T>)
					   std::atexit(&Destroy);
				   }
	);
	assert(value_ != nullptr);
	return *value_;
  }

 private:
  static void Destroy() {
	std::destroy_at(value_.release());
  }

  static std::once_flag ponce_;
  static std::unique_ptr<T> value_;
  DISALLOW_COPY_AND_ASSIGN(Singleton)
};

template<typename T>
std::once_flag Singleton<T>::ponce_;

template<typename T>
std::unique_ptr<T> Singleton<T>::value_ = nullptr;

} // turbo

#endif //TURBONET_SRC_BASE_SINGLETON_H_
